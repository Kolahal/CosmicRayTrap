#include "/share/apps/hdf5/1.8.14/gcc/4.4.7/include/hdf5.h"
#include<iostream>
#include <string>
#define FILE "MagFieldData_array_2cm_grid_gpu.hdf5"

int main(void)
{
	hid_t	file_id, dataset_id;  /* identifiers */
	herr_t	status;
	double	i, j, k, dset_data[501][501][501];
	
	/* Open an existing file. */
	file_id = H5Fopen(FILE, H5F_ACC_RDWR, H5P_DEFAULT);
	
	/* Open an existing dataset. */
	//dataset_id = H5Dopen2(file_id, "/dset", H5P_DEFAULT);
	
	//status = H5Dread(dataset_id, H5T_NATIVE_INT, H5S_ALL, H5S_ALL, H5P_DEFAULT, dset_data);
	
	/* Close the dataset. */
	//status = H5Dclose(dataset_id);
	
	/* Close the file. */
	status = H5Fclose(file_id);
	
	std::cout<<"ok"<<std::endl;
	return 0;
}
