//---------------------------------------------------------------------------
class TProfileWindow : public TStemDialog
{
private:
  static LRESULT __stdcall WndProc(HWND,UINT,WPARAM,LPARAM);
  void ManageWindowClasses(bool);
public:
  TProfileWindow();
  ~TProfileWindow();

  void Show(),Hide();

  void RefreshProfileBox();

  int Width,Height;
  bool Maximized;

  enum eSortMode
  {
	  eSortMode_Alpha,
	  eSortMode_Cycle
  };

  enum eDisplayMode
  {
	  eDisplayMode_Total,
	  eDisplayMode_Frame,
	  eDisplayMode_Frames
  };

  eSortMode	mSortMode;
  eDisplayMode mDisplayMode;
  int	mFrameIndex;
};

