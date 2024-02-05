
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

	void Execute(vtkObject *vtkNotUsed(caller), unsigned long event, void *calldata) override;

	std::string GetErrorMessage() const
	{
		return ErrorMessage;
	}
	
	std::string GetWarningMessage() const
	{
		return WarningMessage;
	}

	void SetView(CVTKView* view)
	{
		theView = view;
	}

private:
	CVTKView* theView = nullptr;
	bool        Error = false;
	bool        Warning = false;
	std::string ErrorMessage;
	std::string WarningMessage;
};


class CVTKView : public CView
{
protected: // create from serialization only
	CVTKView();
	DECLARE_DYNCREATE(CVTKView)

	static bool IsHandledMessage(UINT message);

public:
	~CVTKView() override;
	CNMRIDoc* GetDocument() const;

// Operations
	void RecoverFromWarning();
	void GrabResultsFromDoc();
	void UpdateTransferFunctions();

private:
// Overrides
	void OnDraw(CDC* pDC) override;  // overridden to draw this view
	BOOL OnPreparePrinting(CPrintInfo* pInfo) override;
	void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo) override;
	void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo) override;

// Implementation
	void Pipeline();

#ifdef _DEBUG
	void AssertValid() const override;
	void Dump(CDumpContext& dc) const override;
#endif

	ErrorObserver* errorObserver = nullptr;
	vtkImageData* dataImage = nullptr;

	vtkWin32OpenGLRenderWindow *renWin;
	vtkRenderer *ren = nullptr;
	vtkWin32RenderWindowInteractor *iren = nullptr;

	vtkGPUVolumeRayCastMapper* volumeMapper = nullptr;
	vtkVolume* volume = nullptr;

	vtkSmartPointer<vtkTextActor> textActor;

	unsigned int Width = 0;
	unsigned int Height = 0;
	unsigned int NrFrames = 0;

// Generated message map functions
	afx_msg void OnFilePrintPreview();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	void OnInitialUpdate() override;
	LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam) override;
	afx_msg void OnDestroy();
	//afx_msg void OnTimer(UINT_PTR nIDEvent);
	
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in DFTView.cpp
inline CNMRIDoc* CVTKView::GetDocument() const
   { return reinterpret_cast<CNMRIDoc*>(m_pDocument); }
#endif

