/*
 *  (c) 2004 Iowa State University
 *      see the LICENSE file in the top level directory
 */

/*ееееееееееееееееееееееееееееееееееееееее
	BFiles.h

	Buffered File class definitions

	Brett Bode - June 1996
	Adapted for Std C IO for Rhapsody - 2/98 BMB
  ееееееееееееееееееееееееееееееееееееееее*/
#ifndef __BFILE__	//make sure this header is not already included
#define __BFILE__
#include <stdio.h>

#define	kBufferSize		65536		//default file buffer size
#define kMaxLineLength	180			//arbitrary Max line length (should handle 132 col lines)
 
typedef enum TextFileType {
	kUnknown,
	kMolType,
	kGAMESSlogType,
	kGAMESSIRCType,
	kGAMESSDRCType,
	kGAMESSInputType,
	kXYZType,
	kPDBType,
	kMDLMolFile,
	CMLFile
} TextFileType;

long FindKeyWord(const char *buffer, const char keyword[], long numbyte);
long ReadStringKeyword(const char * Line, const char * Keyword, char * Value);
long ReadLongKeyword(const char * Line, const char * Keyword, long * Value);
long ReadFloatKeyword(const char * Line, const char * Keyword, float * Value);
long ReadBooleanKeyword(const char * Line, const char * Keyword, bool * Value);
long LocateForValue(const char * Line, const char * KeyWord);

class BufferFile {
	private:
#ifdef UseHandles
		Handle		Buffer;
#else
		Ptr			Buffer;
#endif
		long		BufferStart;
		long		ByteCount;
						//Blocks allow the user to restrict reads to a given
						//size below that of the total file length
		long		BlockCount;
		long *		BlockLengths;
		long		BlockArrayAllocation;
		long		BufferPos;
		long		BufferSize;		//Size (in bytes) of IO buffer
		long		LinePos;
#ifdef UseMacIO
		short		FileRefNum;
#else
		FILE *		FilePtr;
#endif
 		short		ColsPerLine;
 		short		IOType;
 		char		EOLchar;
 		bool		DoIt;
			//Internal routine which takes care of the actual reading/writing to disk
 		void AdvanceBuffer(void);
 	public:
 			//Build a read buffer on the specified (already open!) file
#ifdef UseMacIO
 		BufferFile(short TargetFileRef, bool Write);
#else
 		BufferFile(FILE * TargetFileRef, bool Write);
#endif
#ifdef UseHandles
 		BufferFile(Handle	TargetHandle, long HandleSize);
#else
 		BufferFile(Ptr	TargetPtr, long PtrSize);
#endif
 		~BufferFile(void);
		long GetFilePos(void);
		void SetFilePos(long NewPos);
		inline long GetFileLength(void) {return ((ByteCount>GetFilePos())?ByteCount:GetFilePos());};
		TextFileType GetFileType(const char * fileName);
		long FindGroup(char * GroupName);
 		long Read(Ptr Target, long NumBytes);
 		long GetLine(char * Line);
		long PutText(const char * Text);
		void BackupnLines(long nBack);
		void SkipnLines(long nSkip);
		inline bool LocateKeyWord(char Keyword[], long NumByte) {return LocateKeyWord(Keyword, NumByte, -1);};
			//Search the file for the specified keyword until found, EOF, or the limit is reached
			//Returns true or false, the file position upon exit will be the start of the keyword,
			//or the starting position if the keyword is not found.
		bool LocateKeyWord(const char Keyword[], long NumByte, long Limit);
		long FindBlankLine(void);
		long GetNumLines(long size);
 		long BufferSkip(long NumBytes);
 		inline void SetColsPerLine(short newVal) {ColsPerLine = newVal;};
 		inline float GetPercentRead(void) {return (float) GetFilePos()/GetFileLength();};
 		long Write(const char * Source, long NumBytes);
		long WriteLine(Ptr text, bool newline);
 		bool SetOutput(bool State);
 		bool GetOutput(void);
 		void AbnormalCleanup(void);
 		void CloseFile(void);
 		void SetBlockLength(long length);
 		void FinishBlock(void);
		inline long GetFileSize(void) const {return ByteCount;};
 		long BytesLeftInBlock(void) {return (BlockLengths[0]-GetFilePos());};
 };
 
#endif
