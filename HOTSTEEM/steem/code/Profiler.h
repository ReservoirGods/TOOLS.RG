#ifndef	INCLDUED_PROFILER_H
#define	INCLDUED_PROFILER_H


typedef unsigned char			U8;
typedef signed char				S8;
typedef unsigned short			U16;
typedef signed short			S16;
typedef unsigned int			U32;
typedef signed int				S32;
typedef float					F32;
typedef unsigned long long		U64;
typedef signed long long		S64;
typedef double					F64;

namespace	Profiler
{
	class cSymbol
	{
	public:
		char*	mpName;
		U32		mAddress;
		U32		mCycleCount;
	};

	void	Init( void );
	void	DeInit( void );

	void	Start( void );
	void	Stop( void );

	void	Add( unsigned long aAddress, int aCycleCount );
	void	OnVBL( void );

	void	OnPexec( const char * apFileName, unsigned int aBasePageAddress );
	void	OnPterm( void );

	void	Dump( void );
	void	Reset( void );

	unsigned int	GetInstructionCycleCountTotal( unsigned int aAddress );
	unsigned int	GetInstructionCycleCountFrame( unsigned int aAddress );
	const char *	GetSymbolName( unsigned int aAddress );

	unsigned int	GetSymbolCount( void );

	void			FrameCyclesSort( void );
	void			TotalCyclesSort( void );
	void			AlphaSort( void );
	void			AdressSort( void );

	void			SymbolCycleCountsBuildFromTotal( void );
	void			SymbolCycleCountsBuildFromFrame( unsigned int aIndex );

	const cSymbol *	GetpTotalCycleSymbol( unsigned int aIndex );

	void			FrameRingBufferSort( void );
	unsigned int	GetFrameCount( void );
	unsigned int	GetFrameNumber( unsigned int aFrameIndex );
	unsigned int	GetFrameIndex( unsigned int aFrameIndex );
	unsigned int	GetFrameCycleCount( unsigned int aFrameIndex );
}

#endif	INCLDUED_PROFILER_H