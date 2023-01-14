
// VTKView.h : interface of the CVTKView class
//

#pragma once


#ifdef _DEBUG
#define new ::new
#endif

#include <vtkFixedPointVolumeRayCastMapper.h>
//#include <vtkVolumeRayCastCompositeFunction.h>
#include <vtkVolumeProperty.h>
#include <vtkPiecewiseFunction.h>
#include <vtkColorTransferFunction.h>
#include <vtkOutlineFilter.h>
#include <vtkPolyDataMapper.h>
#include <vtkCubeSource.h>

#include <vtkFileOutputWindow.h>

#include <vtkSmartPointer.h>
#include <vtkImageData.h>

#include <vtkWin32OpenGLRenderWindow.h>
#include <vtkWin32RenderWindowInteractor.h>
#include <vtkRenderer.h>

#include <vtkFloatArray.h>
#include <vtkImageData.h>

#include <vtkDataSetMapper.h>

#include <vtkCamera.h>
#include <vtkProperty.h>
#include <vtkInformation.h>

#include <vtkTextActor.h>
#include <vtkTextProperty.h>

#include <vtkGPUVolumeRayCastMapper.h>
#include <vtkWin32OutputWindow.h>
#include <vtkCommand.h>

#include <vtkLight.h>


#ifdef _DEBUG
#undef new
#endif


class CVTKView;

// class stolen from VTK examples and modified

class ErrorObserver : public vtkCommand
{
public:
	ErrorObserver() : theView(nullptr), Error(false), Warning(false), ErrorMessage(""), WarningMessage("") {}
  
	static ErrorObserver *New();

	bool GetError() const
	{
		return Error;
	}
	
	bool GetWarning() const
	{
		return Warning;
	}
	
	void Clear()
	{
		Error = false;
		Warning = false;
		ErrorMessage = "";
		WarningMessage = "";
	}

	virtual void Execute(vtkObject *vtkNotUsed(caller), unsigned long event, void *calldata);

	std::string GetErrorMessage() const
	{
		return ErrorMessage;
	}
	
	std::string GetWarningMessage() const
	{
		return WarningMessage;
	}

	CVTKView* theView;

private:
	bool        Error;
	bool        Warning;
	std::string ErrorMessage;
	std::string WarningMessage;
};


class CVTKView : public CView
{
protected: // create from serialization only
	CVTKView();
	DECLARE_DYNCREATE(CVTKView)

	static bool IsHandledMessage(UINT message);
// Attributes
public:
	CNMRIDoc* GetDocument() const;

// Operations
public:
	ErrorObserver* errorObserver = nullptr;

	vtkImageData* dataImage = nullptr;

	void RecoverFromWarning();

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view

protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CVTKView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	vtkWin32OpenGLRenderWindow *renWin;
	vtkRenderer *ren;
	vtkWin32RenderWindowInteractor *iren;

	vtkGPUVolumeRayCastMapper* volumeMapper;
	vtkVolume* volume;

	vtkSmartPointer<vtkTextActor> textActor;

	unsigned int Width = 0;
	unsigned int Height = 0;
	unsigned int NrFrames = 0;

// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	virtual void OnInitialUpdate();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	void Pipeline();
	afx_msg void OnDestroy();
	//afx_msg void OnTimer(UINT_PTR nIDEvent);
	void GrabResultsFromDoc();

	void UpdateTransferFunctions();
};

#ifndef _DEBUG  // debug version in DFTView.cpp
inline CNMRIDoc* CVTKView::GetDocument() const
   { return reinterpret_cast<CNMRIDoc*>(m_pDocument); }
#endif

