// VesselNess.cpp : Defines the entry point for the console application.
//
#define _CRT_SECURE_NO_DEPRECATE

#include "stdafx.h"
#include "VesselDetector.h"
#include "Validation.h"
#include "Kernel3D.h"
#include "Viewer.h"
#include "RingsDeduction.h"
#include "Image3D.h"
#include "ImageProcessing.h"
#include "Vesselness.h"

Data3D<Vesselness_All> vn_all;
Image3D<short> image_data;

class Preset {
public:
	Preset(){};
	Preset(const string& file, Vec3i& size = Vec3i(0,0,0) ) : file(file), size(size){ };
	string file;
	Vec3i size;
};


int main(int argc, char* argv[])
{
	Validation::Hessian_2D();
	return 0;

	bool flag = false;
	Preset presets[30];
	presets[0] = Preset("vessel3d", Vec3i(585, 525, 892));
	presets[1] = Preset("roi1.data", Vec3i(45, 70, 15));
	presets[2] = Preset("roi2.data", Vec3i(111, 124, 43));
	presets[3] = Preset("roi3", Vec3i(70, 123, 35));
	presets[4] = Preset("roi4", Vec3i(29, 30, 56));
	presets[7] = Preset("roi7", Vec3i(65, 46, 55));
	presets[8] = Preset("roi8.data", Vec3i(102, 96, 34));
	presets[9] = Preset("roi9", Vec3i(129, 135, 97));
	presets[10] = Preset("roi10", Vec3i(51, 39, 38));
	presets[11] = Preset("roi11.data", Vec3i(116, 151, 166));
	presets[12] = Preset("vessel3d.rd.k=19.data", Vec3i(585, 525, 892));
	presets[13] = Preset("roi13.data", Vec3i(238, 223, 481) );
	presets[14] = Preset("roi14.data" );
	presets[15] = Preset("roi15.data" );
	presets[16] = Preset("roi16.data" );
	presets[17] = Preset("roi17.data" );
	
	const Preset& ps = presets[17];
	string data_name = "roi17.partial";
	// string data_name = "temp";


	if( bool isConstructTube = false ) {
		Validation::construct_tube2( image_data );
		//image_data.show("", 50);
		//return 0;
	} 

	/////////////////////////////////////////////////////////////////////////////////////
	//// For tuning parameters
	//flag = image_data.loadROI( ps.file );
	//if( !flag ) return 0;
	//static const int NUM_S = 4;
	//float sigma[] = { 0.3f, 0.5f, 0.7f, 1.5f };
	//// float sigma[] = { 0.5f, 1.5f, 2.5f };
	//int margin = (int) sigma[NUM_S-1]*6 + 20;
	//Vec3i size = image_data.getROI().get_size() - 2* Vec3i(margin, margin, margin);
	//size[0] *= NUM_S;
	//Data3D<Vesselness_All> vn_temp(size);
	//for( int i=0; i<NUM_S; i++ ){ 
	//	VD::compute_vesselness( image_data.getROI(), vn_all, sigma[i], 3.1f, 100.0f );
	//	vn_all.remove_margin( Vec3i(margin+30,margin,margin), Vec3i(margin,margin,margin) );
	//	int x,y,z;
	//	for(z=0;z<vn_all.SZ();z++ ) for(y=0;y<vn_all.SY();y++ ) for(x=0;x<vn_all.SX();x++ ) {
	//		vn_temp.at( x + i*vn_all.SX(), y, z ) = vn_all.at( x, y, z );
	//	}
	//}
	//Viewer::MIP::Multi_Channels( vn_temp, data_name+".vesselness" );
	//return 0;

	if( bool compute = true ) {
		int margin = VD::compute_vesselness( image_data.getROI(), vn_all, 
			// /*sigma from*/ 0.3f, /*sigma to*/ 3.5f, /*sigma step*/ 0.1f );
		    /*sigma from*/ 0.5f, /*sigma to*/ 3.5f, /*sigma step*/ 0.1f );
		
		flag = vn_all.remove_margin( margin ); if( !flag ) return 0;
		vn_all.save( data_name+".vesselness" );
		
		flag = image_data.getROI().remove_margin( margin ); if( !flag ) return 0;
		image_data.getROI().save( data_name + ".data" );

		Viewer::MIP::Multi_Channels( vn_all, data_name + ".vesselness" );
		Viewer::MIP::Single_Channel( image_data.getROI(), data_name + ".data" );
		return 0;

	} else {

		flag = vn_all.load( data_name+".vesselness" );
		if( !flag ) return 0;

		flag = image_data.load( data_name+".data" );
		if( !flag ) return 0;
	}
	

	Data3D<Vesselness_Sig> vn_sig( vn_all );
	//vn_sig.save( data_name+".float5.vesselness" );
	//Viewer::MIP::Multi_Channels( vn_sig, data_name+".float5.vesselness" );
	//Viewer::OpenGL::show_dir( vn_sig );
	//return 0;

	if( bool minimum_spinning_tree = false ) { 
		
		Data3D<Vesselness_Sig> res_mst;
		IP::edge_tracing_mst( vn_all, res_mst, 0.55f, 0.065f  );
		// res_dt.save( data_name + ".dir_tracing.vesselness" );
		Viewer::MIP::Multi_Channels( res_mst, data_name + ".mst" );
		return 0;
	} else {
		Data3D<Vesselness_Sig> res_nms; // result of non-maximum suppression
		IP::non_max_suppress( vn_all, res_nms );
		res_nms.save( data_name + ".non_max_suppress.vesselness" );
		// Viewer::MIP::Multi_Channels( res_nms, data_name + ".non_max_suppress" ); // Visualization using MIP
		//return 0;

		Data3D<Vesselness_Sig> res_rns_et;
		IP::edge_tracing( res_nms, res_rns_et, 0.55f, 0.055f );
		res_rns_et.save( data_name + ".edge_tracing.vesselness" );
		Viewer::MIP::Multi_Channels( res_rns_et, data_name + ".edge_tracing"); // Visualization using MIP
	}

	

	return 0;

	///////////////////////////////////////////////////////////////
	// Ring Recuction by slice
	////////////////////////////////////////////////////////////////
	////Get a clice of data
	//Mat im_slice = image_data.getByZ( 55 );
	//Mat im_no_ring;
	//for( int i=0; i<5; i++ ) {
	//	Validation::Rings_Reduction_Polar_Coordinates( im_slice, im_no_ring, 9 );
	//	im_slice = im_no_ring;
	//}


	return 0;
}
