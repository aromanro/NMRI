#include "stdafx.h"
#include "NMRIFile.h"


NMRIFile::NMRIFile()
	: filterHighFreqs(false), filterLowFreqs(false), NrFrames(0), Width(0), Height(0), themax(0), realData(nullptr), imgData(nullptr), theFrame(nullptr), srcFrame(nullptr), fft(4)
{
}


NMRIFile::~NMRIFile()
{
	delete[] srcFrame;
	delete[] theFrame;
	delete[] realData;
	delete[] imgData;
}


bool NMRIFile::Load(const CString& name)
{
	FILE *f = nullptr;
	if (_wfopen_s(&f, (LPCWSTR)name, L"rb")) return false;

	fread(&NrFrames,sizeof(int),1,f);
	fread(&Width,sizeof(int),1,f);
	fread(&Height,sizeof(int),1,f);

	int Size = Width * Height * NrFrames;

	delete[] realData;
	delete[] imgData;
	delete[] theFrame;
	delete[] srcFrame;

	fft.Clear();

	themax = 0;

	if (Size)
	{
		realData = new float[Size];
		imgData = new float[Size];

		fread(realData, sizeof(float)*Size, 1, f);
		fread(imgData, sizeof(float)*Size, 1, f);

		theFrame = new std::complex<double>[Width*Height];
		srcFrame = new std::complex<double>[Width*Height];
	}
	else
	{
		realData = nullptr;
		imgData = nullptr;
		theFrame = nullptr;
		srcFrame = nullptr;
	}

	fclose(f);


	for (int frame = 0; frame < NrFrames; ++frame)
		for (int x = 0; x < Width; ++x)
			for (int y = 0; y < Height; ++y)
			{
				const std::complex<double> val = GetValue(frame, x, y);

				if (themax < std::norm(val)) themax = std::norm(val);
			}

	return true;
}
