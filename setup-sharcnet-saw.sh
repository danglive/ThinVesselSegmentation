# This file contains important/usefull commands for compiling and running the code on Sharcnet.


###############################################
# Login to Sharcnet
###############################################
ssh saw.sharcnet.ca -l username


###############################################
# Clone the project
###############################################
git clone https://yzhong52@bitbucket.org/yzhong52/thin-structure-segmentation.git




###############################################
# Set up the compiling environment. 
###############################################

# Change compiler
module unload intel/12.1.3
module load gcc/4.8.2

# Python
# Please refer to 'Python Configuration lod.sh'

# OpenCV
module load opencv/2.4.9
export PKG_CONFIG_PATH=/opt/sharcnet/opencv/2.4.9/lib/pkgconfig
pkg-config --modversion opencv
pkg-config --cflags opencv
pkg-config --libs opencv

# Show all modules
module list





###############################################
# Copy File from to Sharcnet using 'scp'
###############################################
scp <filename> <username>@saw.sharcnet.ca:

example: 
# copy a data file (with its readme file) to a excutable bin folder on sharcnet
scp data15.et.vn_sig data15.et.vn_sig.readme.txt yzhong52@saw.sharcnet.ca:/../../scratch/yzhong52/thin-structure-segmentation/bin
scp vessel3d_rd.et.vn_sig vessel3d_rd.et.vn_sig.readme.txt yzhong52@saw.sharcnet.ca:/../../scratch/yzhong52/thin-structure-segmentation/bin
# copy the output directory back to a local computer 
scp -r yzhong52@saw.sharcnet.ca:/../../scratch/yzhong52/thin-structure-segmentation/bin ./from-sharcnet



###############################################
# Submitting a job on sharcnet
###############################################
sqsub -q <Type of program> -n <number of cpus cores> -N <number of nodes> -o <output log file> -r <maximum run time> ./<program executable name> 
# Note: For example, on saw (sharcnet cluster name) there are 8 cores per node. 
sqsub --mpp=2g -q threaded -n 8 -N 1 -o modelfitting.log -r 5h ./modelfitting data15
sqsub --mpp=8g -q threaded -n 16 -N 1 -o modelfitting.log -r 1.5d ./modelfitting vessel3d_rd




