#include "Viewer.h"
#include "Image3D.h"
#include "Vesselness.h"

// Yuchen: Adding glut for OpenCV
#include "gl/glut.h"

// Yuchen: Adding engine for Matlab
#ifdef _CHAR16T
#define CHAR16_T
#endif
#include "engine.h"

namespace Viewer{ 
	namespace OpenGL {

		// global data pointer
		const Data3D<Vesselness_Sig>* ptrVnSig = NULL;
		// Data3D<short>* ptrImage = NULL;
		// setting parameters
		float threshold = 0.0f;
		// Some global variables
		// zooming
		float zoom = 1.0f; 
		// For Camera Rotation
		double camera_angle_h = 0; 
		double camera_angle_v = 0; 
		int drag_x_origin;
		int drag_y_origin;
		int dragging = 0;
		// id for the window that we create
		int window_id;

		//void renderVoxelFunc( const Vec3i& center, const float& thres, int gap = 1){
		//	int x, y, z;
		//	glBegin( GL_POINTS );
		//	glColor3f( 1.0, 1.0, 1.0 );
		//	for( z=0; z < ptrImage->get_size_z(); z+=gap ) {
		//		for( y=0; y < ptrImage->get_size_y(); y+=gap ) {
		//			for( x=0; x < ptrImage->get_size_x(); x+=gap ) {
		//				if( ptrImage->at(x, y, z) > thres ) { 
		//					//// select white color
		//					//glColor3f( 
		//					//	ptrVesselness->at(x,y,z)[0], 
		//					//	ptrVesselness->at(x,y,z)[0], 
		//					//	1.0f - ptrVesselness->at(x,y,z)[0] ); 
		//					// draw the point
		//					glVertex3f( 
		//						zoom*(x-center[0]), 
		//						zoom*(y-center[1]), 
		//						zoom*(z-center[2]));
		//				}
		//			}
		//		}
		//	}
		//	glEnd();
		//}

		void renderDirFunc( void )
		{
			glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

			// calculate the center of the scene
			Vec3i center( ptrVnSig->get_size()/2 );

			// camera
			glLoadIdentity();
			glOrtho( -center[0], center[0], -center[1], center[1], -100000.0f, 100000.0f);
			gluLookAt( /*orgin*/ 0.0, 0.0, 1.0, /*look at*/ 0.0, 0.0, 0.0, /*up vector*/0.0, 1.0, 0.0);

			glRotated( camera_angle_v, 1.0, 0.0, 0.0);
			glRotated( camera_angle_h, 0.0, 1.0, 0.0);

			// draw vessel direction
			glBegin( GL_LINES );
			int gap = 2;
			for( int z=0; z<ptrVnSig->SZ(); z+=gap ) {
				for( int y=0; y<ptrVnSig->SY(); y+=gap ) {
					for( int x=0; x<ptrVnSig->SX(); x+=gap ) {
						if( ptrVnSig->at(x, y, z).rsp > threshold ) { 
							// select line color
							glColor4f( 1.0, 0.0, 0.0, ptrVnSig->at(x,y,z).rsp); 
							// draw line
							glVertex3f( zoom*(x-center[0]), zoom*(y-center[1]), zoom*(z-center[2]) );
							glVertex3f( 
								zoom * ( x - center[0] + ptrVnSig->at(x,y,z)[1] * 5), 
								zoom * ( y - center[1] + ptrVnSig->at(x,y,z)[2] * 5), 
								zoom * ( z - center[2] + ptrVnSig->at(x,y,z)[3] * 5) );
						}
					}
				}
			}
			glEnd();
			glutSwapBuffers();
		}

		void mouse_click(int button, int state, int x, int y) {
			if(button == GLUT_LEFT_BUTTON) {
				if(state == GLUT_DOWN) {
					dragging = 1;
					drag_x_origin = x;
					drag_y_origin = y;
				} else {
					dragging = 0;
				}
			} else if ( button==3 ) {
				// mouse wheel scrolling up
				zoom *= 1.1f;
			} else if ( button==4 ) {
				// mouse wheel scrooling down 
				zoom *= 0.9f;
			}
		}

		void mouse_move(int x, int y) {
			if(dragging) {
				camera_angle_v += (y - drag_y_origin)*0.3;
				camera_angle_h += (x - drag_x_origin)*0.3;
				drag_x_origin = x;
				drag_y_origin = y;
			}
		}

		void reshape( int w, int h )
		{
			glViewport( 0, 0, (GLsizei) w, (GLsizei) h );
			glMatrixMode( GL_PROJECTION );
			glLoadIdentity( );
			glOrtho(0.0, (GLdouble)w, 0.0, (GLdouble)h, -1.0f, 100.0f);
		}

		void key_press( unsigned char key, int x, int y ){
			switch ( key )
			{
			case 27: // Escape key
				glutDestroyWindow ( window_id );
				break;
			}
		}

		void init( void (* renderFunc)( void ) ) 
		{
			int argc = 1;
			char *argv[1] = {(char*)"Something"};
			glutInit(&argc, argv);
			glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGBA );
			glutInitWindowSize( 800, 800 );
			glutInitWindowPosition( 100, 100 );
			window_id = glutCreateWindow( "Visualization of Vesselness" );

			// display
			glutDisplayFunc( renderFunc );
			glutIdleFunc( renderFunc );
			// register mouse fucntions
			glutMouseFunc( mouse_click );
			glutMotionFunc( mouse_move );
			// register keyboard functions
			glutKeyboardFunc( key_press );
			// window resize 
			glutReshapeFunc( reshape );

			glutMainLoop();
		}

		void show_dir( const Data3D<Vesselness_Sig>& vnSig, const float& thres ){
			ptrVnSig = &vnSig;
			threshold = thres;
			smart_return( ptrVnSig->get_size_total(), "Data does not exist." );
			init( renderDirFunc );
		}
	}

	namespace Matlab {
		void plot( Mat_<double> mx, Mat_<double> my )
		{ 
			vector< Mat_<double> > mys( &my, &my+1 );
			plot( mx, mys);
		}

		void plot( Mat_<double> mx, vector< Mat_<double> > mys ){
			for( unsigned int i=0; i<mys.size(); i++ )
			{ 
				if( mx.rows != mys[i].rows ) {
					cout << "Error using ==> plot. " << endl;
					cout << "Vectors must be the same lengths. " << endl;
					return;
				} 
			}
			const int N = mx.rows;

			// open matlab engine
			Engine *ep = engOpen(NULL);
			if ( !ep )
			{
				cout << "Can't start Matlab engine!" <<endl;
				exit(1);
			}

			// The fundamental type underlying MATLAB data
			mxArray *xx = mxCreateDoubleMatrix(1, N, mxREAL); 
			mxArray *yy = mxCreateDoubleMatrix(1, N, mxREAL); 

			// copy data from c++ to matlab data
			memcpy( mxGetPr(xx), mx.data, N * sizeof(double) ); 
			memcpy( mxGetPr(yy), mys[0].data, N * sizeof(double) ); 

			// draw the plot
			engPutVariable( ep, "xx", xx ); 
			engPutVariable( ep, "yy", yy ); 
			
			engEvalString(ep, "figure(1);");
			engEvalString(ep, "hFig = figure(1);;");
			engEvalString(ep, "set(gcf,'PaperPositionMode','auto');");
			engEvalString(ep, "set(hFig, 'Position', [100 100 900 300]);");
			engEvalString(ep, "hFig = plot(xx, yy);");
			engEvalString(ep, "set(gca, 'XTickLabel', num2str(get(gca,'XTick')','%d'));");
			engEvalString(ep, "set(gca, 'YTickLabel', num2str(get(gca,'YTick')','%d'));");

			// release data
			mxDestroyArray(xx); 
			mxDestroyArray(yy); 

			system("pause");

			// close matlab window
			engClose(ep);
		}
	}
}

