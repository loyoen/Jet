// taskServer.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "taskServer.h"
#include "taskServerDlg.h"

#include "MgrMessage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CtaskServerApp

BEGIN_MESSAGE_MAP(CtaskServerApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CtaskServerApp construction

CtaskServerApp::CtaskServerApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CtaskServerApp object

CtaskServerApp theApp;


// CtaskServerApp initialization

BOOL CtaskServerApp::checkInstance( )
{
	::CreateMutex( NULL, TRUE, m_pszExeName );
	if ( GetLastError() == ERROR_ALREADY_EXISTS )
	{
		// Find out previous application's main window
		CWnd *	pPrevWnd;

		pPrevWnd	= CWnd::GetDesktopWindow()->GetWindow( GW_CHILD );
		while ( pPrevWnd )
		{
			// Does this window have the 'previous instance tag'set?
			if ( ::GetProp( pPrevWnd->GetSafeHwnd(), m_pszExeName ) )
			{
				// Found window, now set focus to the window
				// First restore window if it is currently iconic
				if ( pPrevWnd->IsIconic() )
					pPrevWnd->ShowWindow( SW_RESTORE );

				// set focus to main window
				pPrevWnd->SetForegroundWindow( );

				// If window has a pop-up window, set focus to pop-up
				pPrevWnd->GetLastActivePopup()->SetForegroundWindow();

				return	TRUE;
			}

			// get next window in list
			pPrevWnd	= pPrevWnd->GetWindow( GW_HWNDNEXT );
		}

		TRACE ( "Could not find previous instance main window!\n" );
		return	TRUE;
	}
	
	return	FALSE;
}


extern void	*pMainDlg;

BOOL CtaskServerApp::InitInstance()
{
	if ( checkInstance() )	// if there is an instance running, just quit this one...
	{
		return FALSE;
	}

	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Jicheng @ Wuhan University"));

	CtaskServerDlg dlg;
	m_pMainWnd = &dlg;
	pMainDlg = m_pMainWnd;

	CMgrMessage::getManager ();

	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

int CtaskServerApp::ExitInstance()
{
	// TODO: Add your specialized code here and/or call the base class
	CMgrMessage	*pMgr;

	pMgr	= CMgrMessage::getManager ();

	delete	pMgr;
	CMgrMessage::pThis	= NULL;

	return CWinApp::ExitInstance();
}
