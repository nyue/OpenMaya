#ifndef TOOLS_BINMESH_H
#define TOOLS_BINMESH_H

#include <maya/MPxFileTranslator.h>

class polyWriter;
class MDagPath;
class MFnDagNode;

class BinMeshTranslator:public MPxFileTranslator {

	public:
								BinMeshTranslator();
		virtual					~BinMeshTranslator();

		virtual MStatus			writer (const MFileObject& file,
										const MString& optionsString,
										MPxFileTranslator::FileAccessMode mode);
		virtual MStatus			reader ( const MFileObject& file,
										const MString& optionsString,
										FileAccessMode mode);
		virtual bool			haveWriteMethod () const;
		virtual bool			haveReadMethod () const;
		virtual	bool			canBeOpened () const;
		static void*			creator(); 

		virtual MString			defaultExtension () const;


	protected:	
		MStatus					exportAll();
		MStatus					exportSelection();
};

#endif