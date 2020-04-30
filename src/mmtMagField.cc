//#include "/share/apps/python/2.7.3/include/python2.7/Python.h"
#include "/usr/include/python3.6m/Python.h"
#include "mmtMagField.hh"
#include "G4GenericMessenger.hh"
#include "G4SystemOfUnits.hh"
#include "globals.hh"
#include "iostream"
#include <cstring>
#include <sstream>
#include "vector"
#include "mmtMagField.hh"
//#include "g4hdf5.hh"
#include "G4GenericMessenger.hh"
#include "G4SystemOfUnits.hh"
#include "globals.hh"
#include <iostream>
#include <fstream>
using namespace std;

mmtMagneticField::mmtMagneticField()
{
	G4cout<<"mmtMagneticField::mmtMagneticField(): Ctr"<<G4endl;
        //Py_Initialize();
	G4cout<< "Magnetic field class initialized" << G4endl;
}

mmtMagneticField::~mmtMagneticField()
{
	//Py_Finalize();
}

void mmtMagneticField::GetFieldValue(const G4double* point,double *bField) const
{
	
	//Py_SetProgramName("abc");
	//Py_Initialize();
	/*
	//G4cout<<"Accessing GetFieldValue() "<<G4endl;
	// default values in mm
	double xi = point[0];
	double yj = point[1];
	double zk = point[2];
	
	std::vector<double> current_sim_point;
	current_sim_point.clear();
	current_sim_point.push_back(xi);
	current_sim_point.push_back(yj);
	current_sim_point.push_back(zk);
	
	//check the position in cm 
	//convert to cm in embedded python script, magnetic field map in cm scale
	G4cout<<"xi "<<1.e-1*xi<<", yj "<<1.e-1*yj<<", zk "<<1.e-1*zk<<G4endl;
	
	string path = "/home/kolahalb/Works/Computation/Projects/MMT/MMTSim/MMT/src/";
	PyRun_SimpleString("import sys");
	//PyRun_SimpleString("sys.path.append(\".\")");
	string str = "sys.path.append('" + path + "')";
	//string str = "sys.path.insert(0,'/people/bhat731/CosmicMuTrap/SimBoxMMT/MMT/MMTSim/src/')";
	PyRun_SimpleString(str.c_str());
	
	//string interpolator_program = "pyinterpolate";
	
	//PyObject* pModule = PyImport_ImportModule("pyinterpolate");
	//PyObject* pModule = PyImport_ImportModuleNoBlock("pyinterpolate");
	
	PyObject* pName = PyUnicode_DecodeFSDefault((char*)"pyinterpolate");
	//PyObject* pName = PyString_FromString((char*)"pyinterpolate");
	
	PyObject* pModule = PyImport_Import(pName);
	
	//Py_DECREF(pName);
	
	if (pModule != NULL)
	{
		//cout<<"pMudile !NULL "<<endl;
		
		PyObject* pFunc = PyObject_GetAttrString(pModule, "Field_interpolator");
		if (pFunc)
		{
			//cout<<"The progrm can see Field_interpolator function, now pass arguments"<<endl;
			
			if (pFunc && PyCallable_Check(pFunc))
			{
				//cout<<"paases callable check "<<endl;
				
				PyObject* pArgs = PyTuple_New(3);
				for (int k=0; k<3; k++)
				{
					PyObject* pPoint = PyFloat_FromDouble(current_sim_point[k]);
					if (!pPoint)
					{
						Py_DECREF(pArgs);
						Py_DECREF(pModule);
					}
					PyTuple_SetItem(pArgs, k, pPoint);
				}
				
				PyObject* pValue = PyObject_CallObject(pFunc, pArgs);
				Py_DECREF(pArgs);
				
				if (pValue != NULL)
				{
					for (Py_ssize_t k = 0; k < PyList_Size(pValue); k++)
					{
						PyObject* interpolated_component_Bfield = PyList_GetItem(pValue,k);
						bField[k] = PyFloat_AsDouble(interpolated_component_Bfield);
						G4cout<<"bField[k] "<<bField[k]<<G4endl;
					}
					Py_DECREF(pValue);
				}
				else
				{
					Py_DECREF(pFunc);
					Py_DECREF(pModule);
					
					PyErr_Print();
					fprintf(stderr,"Call failed\n");
				}
				pValue = NULL;
			}
			else
			{
				if (PyErr_Occurred())
					PyErr_Print();
				fprintf(stderr, "Cannot find function Field_interpolator");
			}
			Py_XDECREF(pFunc);
			Py_DECREF(pModule);
		}
		else
		{
			PyErr_Print();
			fprintf(stderr, "Failed to load module");
		}
	}
	else
	{
		PyErr_Print();
		G4cout<<"could not open pModule"<<endl;
	}
	*/
	
	bField[0] = 0.;
	bField[1] = 0.;
	bField[2] = 1.0*tesla;
	
	bField[3] = 0.0;
	bField[4] = 0.0;
	bField[5] = 0.0;
	
}
