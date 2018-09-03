/*---------------------------------------------------------------------------
FILE: historylist.cpp
MODULE: Steem
CONDITION: _DEBUG_BUILD
DESCRIPTION: The history list window in the debug build that shows a list of
recently executed commands.
---------------------------------------------------------------------------*/

U32	gProfilerWindowSymbolAddress = 0;

//---------------------------------------------------------------------------
TProfileWindow::TProfileWindow()
{
  Width=360+GetSystemMetrics(SM_CXFRAME)*2;
  Height=360+GetSystemMetrics(SM_CYFRAME)*2+GetSystemMetrics(SM_CYCAPTION);
  Left=(GetSystemMetrics(SM_CXSCREEN)-Width)/2;
  Top=(GetSystemMetrics(SM_CYSCREEN)-Height)/2;
  mSortMode = eSortMode_Cycle;
  mDisplayMode = eDisplayMode_Frames;
  mFrameIndex = 0;
}
//---------------------------------------------------------------------------
void TProfileWindow::ManageWindowClasses(bool Unreg)
{
  char *ClassName="Steem Profiler";
  if (Unreg){
    UnregisterClass(ClassName,Inst);
  }else{
    RegisterMainClass(WndProc,ClassName,RC_ICO_STCLOSE);
  }
}
//---------------------------------------------------------------------------
TProfileWindow::~TProfileWindow()
{
  if (IsVisible()) Hide();
}
//---------------------------------------------------------------------------
void TProfileWindow::Show()
{
  if (Handle!=NULL){
    ShowWindow(Handle,SW_SHOWNORMAL);
    SetForegroundWindow(Handle);
    return;
  }

  ManageWindowClasses(SD_REGISTER);
  Handle=CreateWindowEx(WS_EX_CONTROLPARENT | WS_EX_TOOLWINDOW,
                          "Steem Profiler","Profiler",
                          WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_SIZEBOX | WS_MAXIMIZEBOX,
                          Left,Top,Width,Height,
                          NULL,NULL,HInstance,NULL);
  if (HandleIsInvalid()){
    ManageWindowClasses(SD_UNREGISTER);
    return;
  }

  SetWindowLong(Handle,GWL_USERDATA,(long)this);

  HWND hLB=CreateWindowEx(512,"ListBox","Profile",WS_CHILD | WS_VISIBLE | WS_VSCROLL | LBS_USETABSTOPS |
                            WS_TABSTOP | LBS_NOINTEGRALHEIGHT | LBS_HASSTRINGS | LBS_NOTIFY,
                            10,10,280,250,Handle,(HMENU)100,HInstance,NULL);
  INT Tabs[1]={(GetTextSize(Font,"X  ").Width*4)/LOWORD(GetDialogBaseUnits())};
  SendMessage(hLB,LB_SETTABSTOPS,1,LPARAM(Tabs));

  CreateWindow("Button","Show Instructions",WS_CHILDWINDOW | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON,
                  10,267,135,23,Handle,(HMENU)101,HInstance,NULL);

  CreateWindow("Button","Refresh",WS_CHILDWINDOW | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON,
                  155,267,135,23,Handle,(HMENU)102,HInstance,NULL);

  CreateWindow("Button","Cycle Sort",WS_CHILDWINDOW | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON,
	  10,300,135,23,Handle,(HMENU)103,HInstance,NULL);

  CreateWindow("Button","Alpha Sort",WS_CHILDWINDOW | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON,
	  155,300,135,23,Handle,(HMENU)104,HInstance,NULL);


  CreateWindow("Button","Frames",WS_CHILDWINDOW | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON,
	  10,333,135,23,Handle,(HMENU)105,HInstance,NULL);

  CreateWindow("Button","Total",WS_CHILDWINDOW | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON,
	  155,333,135,23,Handle,(HMENU)106,HInstance,NULL);

  SetWindowAndChildrensFont(Handle,Font);

  RefreshProfileBox();

  Focus=GetDlgItem(Handle,100);

  ShowWindow(Handle,SW_SHOW);
  SetFocus(Focus);
}
//---------------------------------------------------------------------------
void TProfileWindow::Hide()
{
  if (Handle==NULL) return;

  ShowWindow(Handle,SW_HIDE);

  DestroyWindow(Handle);Handle=NULL;
  ManageWindowClasses(SD_UNREGISTER);
}
//---------------------------------------------------------------------------
void TProfileWindow::RefreshProfileBox()
{
  HWND Win=GetDlgItem(Handle,100);
  SendMessage(Win,LB_RESETCONTENT,0,0);

  if( mDisplayMode == eDisplayMode_Frames )
  {
	  Profiler::FrameRingBufferSort();

		if( mSortMode == eSortMode_Cycle )
		{
			Profiler::FrameCyclesSort();
		}

	  unsigned int lFrameCount = Profiler::GetFrameCount();
	  for( int f=lFrameCount-1; f>=0; f-- )
	  {
		  char	lInfoText[ 1024 ];
		  sprintf( lInfoText, "%16d  Frame %d", Profiler::GetFrameCycleCount( f ), Profiler::GetFrameNumber( f ) );

		  SendMessage(Win,LB_INSERTSTRING,0,(long)(&lInfoText[0]) );
		  SendMessage(Win,LB_SETITEMDATA,0,f);
	  }

  }
  else
  {
	  if( eDisplayMode_Frame == mDisplayMode )
	  {
		  Profiler::FrameRingBufferSort();
		  Profiler::SymbolCycleCountsBuildFromFrame( mFrameIndex );
	  }
	  else
	  {
		  Profiler::SymbolCycleCountsBuildFromTotal();
	  }

	  if( mSortMode == eSortMode_Cycle )
	  {
		  Profiler::TotalCyclesSort();
	  }
	  else
	  {
		  Profiler::AlphaSort();
	  }
	  unsigned int lSymCount = Profiler::GetSymbolCount();
	  for( unsigned int s=0; s<lSymCount; s++ )
	  {
		  const Profiler::cSymbol * lpS = Profiler::GetpTotalCycleSymbol( (lSymCount)-(s+1) );
		  char	lInfoText[ 1024 ];
		  sprintf( lInfoText, "%16d  %s", lpS->mCycleCount, lpS->mpName );

		  SendMessage(Win,LB_INSERTSTRING,0,(long)(&lInfoText[0]) );
		  SendMessage(Win,LB_SETITEMDATA,0,lpS->mAddress);

	  }

  }
  RECT rc;
  GetClientRect(Win,&rc);
//  int Selected=SendMessage(Win,LB_GETCOUNT,0,0)-1;
  int Selected=0;
  SendMessage(Win,LB_SETCURSEL,Selected,0);
  SendMessage(Win,LB_SETTOPINDEX,max(Selected-(rc.bottom/SendMessage(Win,LB_GETITEMHEIGHT,0,0)),(long)0),0);
//  SendMessage(Win,WM_SETFONT,WPARAM(debug_monospace_disa ? GetStockObject(ANSI_FIXED_FONT):Font),TRUE);
  SendMessage(Win,WM_SETFONT,WPARAM(GetStockObject(ANSI_FIXED_FONT)),TRUE);
}
//---------------------------------------------------------------------------
#define GET_THIS This=(TProfileWindow*)GetWindowLong(Win,GWL_USERDATA);

LRESULT __stdcall TProfileWindow::WndProc(HWND Win,UINT Mess,WPARAM wPar,LPARAM lPar)
{
  LRESULT Ret=DefStemDialogProc(Win,Mess,wPar,lPar);
  if (StemDialog_RetDefVal) return Ret;

  TProfileWindow *This;
  switch (Mess){
    case WM_COMMAND:
      if (LOWORD(wPar)==100){
        if (HIWORD(wPar)==LBN_DBLCLK){
			GET_THIS;
          int Selected=SendMessage(GetDlgItem(Win,100),LB_GETCURSEL,0,0);
		  if( eDisplayMode_Frames == This->mDisplayMode )
		  {
			  This->mFrameIndex = Profiler::GetFrameIndex( Selected );
			  This->mDisplayMode = eDisplayMode_Frame;
			  This->RefreshProfileBox();
		  }
		  else
		  {
			  gProfilerWindowSymbolAddress = SendMessage(GetDlgItem(Win,100),LB_GETITEMDATA,Selected,0);
			  PostMessage(DWin,WM_COMMAND,16999,0);
		  }
        }
      }else if (LOWORD(wPar)==101){
        if (HIWORD(wPar)==BN_CLICKED){
			int Selected=SendMessage(GetDlgItem(Win,100),LB_GETCURSEL,0,0);
			gProfilerWindowSymbolAddress = SendMessage(GetDlgItem(Win,100),LB_GETITEMDATA,Selected,0);
			PostMessage(DWin,WM_COMMAND,16999,0);
        }
      }else if (LOWORD(wPar)==102){
        if (HIWORD(wPar)==BN_CLICKED){
          GET_THIS;
          This->RefreshProfileBox();
        }
      } else if (LOWORD(wPar)==103){
		  GET_THIS;
		  This->mSortMode = eSortMode_Cycle;
		  This->RefreshProfileBox();
	  } else if (LOWORD(wPar)==104){
		  GET_THIS;
		  This->mSortMode = eSortMode_Alpha;
		  This->RefreshProfileBox();
  } else if (LOWORD(wPar)==105){
	  GET_THIS;
	  This->mDisplayMode = eDisplayMode_Frames;
	  This->RefreshProfileBox();
  } else if (LOWORD(wPar)==106){
	  GET_THIS;
	  This->mDisplayMode = eDisplayMode_Total;
	  This->RefreshProfileBox();
  }
      break;
    case WM_SIZE:
      SetWindowPos(GetDlgItem(Win,100),0,0,0,LOWORD(lPar)-20,HIWORD(lPar)-110,SWP_NOMOVE | SWP_NOZORDER);

	  SetWindowPos(GetDlgItem(Win,101),0,10,              HIWORD(lPar)-90,(LOWORD(lPar)/2)-15,23,SWP_NOZORDER);
      SetWindowPos(GetDlgItem(Win,102),0,LOWORD(lPar)/2+5,HIWORD(lPar)-90,(LOWORD(lPar)/2)-15,23,SWP_NOZORDER);

	  SetWindowPos(GetDlgItem(Win,103),0,10,              HIWORD(lPar)-60,(LOWORD(lPar)/2)-15,23,SWP_NOZORDER);
	  SetWindowPos(GetDlgItem(Win,104),0,LOWORD(lPar)/2+5,HIWORD(lPar)-60,(LOWORD(lPar)/2)-15,23,SWP_NOZORDER);

	  SetWindowPos(GetDlgItem(Win,105),0,10,              HIWORD(lPar)-30,(LOWORD(lPar)/2)-15,23,SWP_NOZORDER);
	  SetWindowPos(GetDlgItem(Win,106),0,LOWORD(lPar)/2+5,HIWORD(lPar)-30,(LOWORD(lPar)/2)-15,23,SWP_NOZORDER);

      GET_THIS;
      if (IsIconic(Win)==0){
        if (IsZoomed(Win)==0){
          This->Maximized=0;

          RECT rc;GetWindowRect(Win,&rc);
          This->Left=rc.left;This->Top=rc.top;
          This->Width=rc.right-rc.left;This->Height=rc.bottom-rc.top;
        }else{
          This->Maximized=true;
        }
      }
      break;
    case WM_GETMINMAXINFO:
      ((MINMAXINFO*)lPar)->ptMinTrackSize.x=200+GetSystemMetrics(SM_CXFRAME)*2+GetSystemMetrics(SM_CXVSCROLL);
      ((MINMAXINFO*)lPar)->ptMinTrackSize.y=100+GetSystemMetrics(SM_CYCAPTION)+GetSystemMetrics(SM_CYFRAME)*2;
      break;
    case WM_CLOSE:
      GET_THIS;
      This->Hide();
      return 0;
  }
  return DefWindowProc(Win,Mess,wPar,lPar);
}

#undef GET_THIS

