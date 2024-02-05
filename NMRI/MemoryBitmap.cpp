#include "stdafx.h"
#include "MemoryBitmap.h"


#include <utility>
#include <cassert>

MemoryBitmap::MemoryBitmap(const MemoryBitmap& other) // copy constructor
{
	if (other.data)
	{
		m_width = other.m_width;
		m_height = other.m_height;
		int size = GetStrideLength() * m_height;
		data = new unsigned char[size];
		memcpy(data, other.data, size);
	}
	else {
		data = nullptr;
		m_width = m_height = 0;
	}
}

MemoryBitmap::MemoryBitmap(MemoryBitmap&& other) noexcept // move constructor
	: data(other.data), m_width(other.m_width), m_height(other.m_height)
{
	other.data = nullptr;
	other.m_height = other.m_width = 0;
}

MemoryBitmap& MemoryBitmap::operator=(const MemoryBitmap& other) //copy assignment operator
{
	MemoryBitmap temp(other);

	*this = std::move(temp);

	return *this;
}

MemoryBitmap& MemoryBitmap::operator=(MemoryBitmap&& other) noexcept // move assignment operator
{
	delete[] data;

	m_width = other.m_width;
	m_height = other.m_height;

	data = other.data;

	other.m_height = other.m_width = 0;
	other.data = nullptr;

	return *this;
}

MemoryBitmap::~MemoryBitmap() noexcept
{
	delete[] data;
}


void MemoryBitmap::SetSize(int width, int height)
{
	assert(width != 0 && height != 0);

	if (m_width != width || m_height != height)
	{
		delete[] data;

		m_width = width;
		m_height = height;

		data = new unsigned char[static_cast<size_t>(GetStrideLength()) * height];
	}
}



void MemoryBitmap::SetMatrix(const double* results, int Width, int Height)
{
	if (Width == 0 || Height == 0 || !results) return;

	SetSize(Width, Height);

	int stride = GetStrideLength();

	

	for (int i = 0; i < Height; ++i)
	{
		const int line = (Height - i - 1) * stride;

		for (int j = 0; j < Width; ++j)
		{
			int pos = line + 3 * j;

			const double val = results[Width*i + j];

			data[pos] = static_cast<unsigned char>(val * 255.);
			data[pos + 1] = data[pos];
			data[pos + 2] = data[pos];
		}
	}
}

void MemoryBitmap::SetMatrix(const std::complex<double>* results, int Width, int Height)
{
	if (Width == 0 || Height == 0 || !results) return;

	SetSize(Width, Height);

	const int stride = GetStrideLength();

	for (int i = 0; i < Height; ++i)
	{
		const int line = (Height - i - 1) * stride;

		for (int j = 0; j < Width; ++j)
		{
			const int pos = line + 3 * j;

			const double val = std::abs(results[Width*i + j]);

			data[pos] = static_cast<unsigned char>(val * 255.);
			data[pos + 1] = data[pos];
			data[pos + 2] = data[pos];
		}
	}
}

void MemoryBitmap::Draw(CDC* pDC) const
{
	BITMAPINFO bmi;
	ZeroMemory(&bmi, sizeof(BITMAPINFOHEADER));
	
	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth = m_width;
	bmi.bmiHeader.biHeight = m_height;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = 24;
	bmi.bmiHeader.biCompression = BI_RGB;

	CBitmap bitmap;
	
	bitmap.CreateCompatibleBitmap(pDC, m_width, m_height);
	::SetDIBits(pDC->GetSafeHdc(), bitmap, 0, m_height, data, &bmi, DIB_RGB_COLORS);
	CDC dcMemory;
	dcMemory.CreateCompatibleDC(pDC);
	CBitmap * pOldBitmap = dcMemory.SelectObject(&bitmap);
	pDC->BitBlt(0, 0, m_width, m_height, &dcMemory, 0, 0, SRCCOPY);
	dcMemory.SelectObject(pOldBitmap);
}

void MemoryBitmap::Draw(CDC* pDC, CRect& rect, int origWidth, int origHeight) const
{
	BITMAPINFO bmi;
	ZeroMemory(&bmi, sizeof(BITMAPINFOHEADER));

	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth = m_width;
	bmi.bmiHeader.biHeight = m_height;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = 24;
	bmi.bmiHeader.biCompression = BI_RGB;

	CBitmap bitmap;

	bitmap.CreateCompatibleBitmap(pDC, m_width, m_height);
	::SetDIBits(pDC->GetSafeHdc(), bitmap, 0, m_height, data, &bmi, DIB_RGB_COLORS);
	CDC dcMemory;
	dcMemory.CreateCompatibleDC(pDC);
	CBitmap * pOldBitmap = dcMemory.SelectObject(&bitmap);
	pDC->StretchBlt(rect.left, rect.top, rect.Width(), rect.Height(), &dcMemory, origWidth ? (m_width - origWidth)/2 : 0, origHeight ? (m_height - origHeight)/2 : 0, origWidth ? origWidth : m_width, origHeight ? origHeight : m_height, SRCCOPY);
	dcMemory.SelectObject(pOldBitmap);
}




