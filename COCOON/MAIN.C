/* ###################################################################################
#  INCLUDES
################################################################################### */

#include	<GODLIB\ASSERT\ASSERT.H>
#include	<GODLIB\DRIVE\DRIVE.H>
#include	<GODLIB\FILE\FILE.H>
#include	<GODLIB\GEMDOS\GEMDOS.H>
#include	<GODLIB\SYSTEM\SYSTEM.H>
#include	<GODLIB\XBIOS\XBIOS.H>


/* ###################################################################################
#  DEFINES
################################################################################### */

#define	dCOCOON_SYSVEC_STARTADR	0x0
#define	dCOCOON_SYSVEC_ENDADR	0x200
#define	dCOCOON_SYSVEC_LIMIT	((dCOCOON_SYSVEC_ENDADR-dCOCOON_SYSVEC_STARTADR)/4)

#define	dCOCOON_SYSVAR_STARTADR	0x200
#define	dCOCOON_SYSVAR_ENDADR	0x800
#define	dCOCOON_SYSVAR_LIMIT	((dCOCOON_SYSVAR_ENDADR-dCOCOON_SYSVAR_STARTADR)/2)

#define	dCOCOON_YM_LIMIT		16

enum
{
	eCOCOON_SIZE_BYTE,
	eCOCOON_SIZE_WORD,
	eCOCOON_SIZE_LONG
};

enum
{
	eCOCOON_MMU_FFFF8001,
	eCOCOON_MMU_LIMIT
};

enum
{
	eCOCOON_BUSCONTROL_FFFF8007,
	eCOCOON_BUSCONTROL_LIMIT
};

enum
{
	eCOCOON_SHIFTER_FFFF8201,
	eCOCOON_SHIFTER_FFFF8203,
	eCOCOON_SHIFTER_FFFF820A,
	eCOCOON_SHIFTER_FFFF8260,

	eCOCOON_SHIFTER_LIMIT,
};

enum
{
	eCOCOON_SHIFTERSTE_FFFF8201,
	eCOCOON_SHIFTERSTE_FFFF8203,
	eCOCOON_SHIFTERSTE_FFFF820A,
	eCOCOON_SHIFTERSTE_FFFF820D,
	eCOCOON_SHIFTERSTE_FFFF820F,
	eCOCOON_SHIFTERSTE_FFFF8260,
	eCOCOON_SHIFTERSTE_FFFF8265,

	eCOCOON_SHIFTERSTE_LIMIT,
};

enum
{
	eCOCOON_MFP_FFFFFA01,
	eCOCOON_MFP_FFFFFA03,
	eCOCOON_MFP_FFFFFA05,
	eCOCOON_MFP_FFFFFA07,
	eCOCOON_MFP_FFFFFA09,
	eCOCOON_MFP_FFFFFA0B,
	eCOCOON_MFP_FFFFFA0D,
	eCOCOON_MFP_FFFFFA0F,
	eCOCOON_MFP_FFFFFA11,
	eCOCOON_MFP_FFFFFA13,
	eCOCOON_MFP_FFFFFA15,
	eCOCOON_MFP_FFFFFA17,
	eCOCOON_MFP_FFFFFA19,
	eCOCOON_MFP_FFFFFA1B,
	eCOCOON_MFP_FFFFFA1D,
	eCOCOON_MFP_FFFFFA1F,
	eCOCOON_MFP_FFFFFA21,
	eCOCOON_MFP_FFFFFA23,
	eCOCOON_MFP_FFFFFA25,
	eCOCOON_MFP_FFFFFA27,
	eCOCOON_MFP_FFFFFA29,
	eCOCOON_MFP_FFFFFA2B,
	eCOCOON_MFP_FFFFFA2D,
	eCOCOON_MFP_FFFFFA2F,

	eCOCOON_MFP_LIMIT
};

enum
{
	eCOCOON_VIDEL_FFFF8006,
	eCOCOON_VIDEL_FFFF8201,
	eCOCOON_VIDEL_FFFF8203,
	eCOCOON_VIDEL_FFFF820D,
	eCOCOON_VIDEL_FFFF820E,
	eCOCOON_VIDEL_FFFF8210,
	eCOCOON_VIDEL_FFFF8260,
	eCOCOON_VIDEL_FFFF8265,
	eCOCOON_VIDEL_FFFF8266,
	eCOCOON_VIDEL_FFFF8280,
	eCOCOON_VIDEL_FFFF8282,
	eCOCOON_VIDEL_FFFF8284,
	eCOCOON_VIDEL_FFFF8286,
	eCOCOON_VIDEL_FFFF8288,
	eCOCOON_VIDEL_FFFF828A,
	eCOCOON_VIDEL_FFFF828C,
	eCOCOON_VIDEL_FFFF8290,
	eCOCOON_VIDEL_FFFF82A0,
	eCOCOON_VIDEL_FFFF82A2,
	eCOCOON_VIDEL_FFFF82A4,
	eCOCOON_VIDEL_FFFF82A6,
	eCOCOON_VIDEL_FFFF82A8,
	eCOCOON_VIDEL_FFFF82AA,
	eCOCOON_VIDEL_FFFF82AC,
	eCOCOON_VIDEL_FFFF82C0,
	eCOCOON_VIDEL_FFFF82C2,

	eCOCOON_VIDEL_LIMIT
};

enum
{
	eCOCOON_DMASOUNDSTE_FFFF8901,
	eCOCOON_DMASOUNDSTE_FFFF8921,

	eCOCOON_DMASOUNDSTE_LIMIT
};

enum
{
	eCOCOON_DMASOUNDFALCON_FFFF8900,
	eCOCOON_DMASOUNDFALCON_FFFF8901,
	eCOCOON_DMASOUNDFALCON_FFFF8920,
	eCOCOON_DMASOUNDFALCON_FFFF8921,

	eCOCOON_DMASOUNDFALCON_LIMIT
};

enum
{
	eCOCOON_CROSSBAR_FFFF8930,
	eCOCOON_CROSSBAR_FFFF8932,
	eCOCOON_CROSSBAR_FFFF8934,
	eCOCOON_CROSSBAR_FFFF8935,
	eCOCOON_CROSSBAR_FFFF8936,
	eCOCOON_CROSSBAR_FFFF8937,
	eCOCOON_CROSSBAR_FFFF8938,
	eCOCOON_CROSSBAR_FFFF8939,
	eCOCOON_CROSSBAR_FFFF893A,
	eCOCOON_CROSSBAR_FFFF893C,
	eCOCOON_CROSSBAR_FFFF8941,
	eCOCOON_CROSSBAR_FFFF8943,

	eCOCOON_CROSSBAR_LIMIT
};

enum
{
	eCOCOON_MCH_ST	   = (1<<MCH_ST),
	eCOCOON_MCH_STE    = (1<<MCH_STE),
	eCOCOON_MCH_TT     = (1<<MCH_TT),
	eCOCOON_MCH_FALCON = (1<<MCH_FALCON),
};

enum
{
	eCOCOON_HW_MMU,
	eCOCOON_HW_BUSCONTROL,
	eCOCOON_HW_SHIFTER,
	eCOCOON_HW_SHIFTERSTE,
	eCOCOON_HW_MFP,
	eCOCOON_HW_VIDEL,
	eCOCOON_HW_DMASOUNDSTE,
	eCOCOON_HW_DMASOUNDFALCON,
	eCOCOON_HW_CROSSBAR,

	eCOCOON_HW_LIMIT
};


/* ###################################################################################
#  STRUCTS
################################################################################### */


typedef	struct
{
	U32	SysVecs[ dCOCOON_SYSVEC_LIMIT ];
	U16	SysVars[ dCOCOON_SYSVAR_LIMIT ];
	U32	SysVecsNew[ dCOCOON_SYSVEC_LIMIT ];
	U16	SysVarsNew[ dCOCOON_SYSVAR_LIMIT ];
} sCocoon;

typedef	struct
{
	U32	Address;
	U32	Size;
	U32	Value;
	U32	NewValue;
} sCocoonMarker;

typedef	struct
{
	sCocoonMarker *	pMarker;
	U32				Count;
	U32				MachineTypes;
} sCocoonMarkerHeader;

/* ###################################################################################
#  DATA
################################################################################### */

sCocoonMarker	gCocoonMMU[ eCOCOON_MMU_LIMIT ] =
{
	{	0xFFFF8001L, eCOCOON_SIZE_BYTE, 0,	0	}
};

sCocoonMarker	gCocoonBusControl[ eCOCOON_BUSCONTROL_LIMIT ] =
{
	{	0xFFFF8007L, eCOCOON_SIZE_BYTE, 0,	0	}
};

sCocoonMarker	gCocoonMFP[ eCOCOON_MFP_LIMIT ] =
{
	{	0xFFFFFA01L, eCOCOON_SIZE_BYTE, 0, 0	},
	{	0xFFFFFA03L, eCOCOON_SIZE_BYTE, 0, 0	},
	{	0xFFFFFA05L, eCOCOON_SIZE_BYTE, 0, 0	},
	{	0xFFFFFA07L, eCOCOON_SIZE_BYTE, 0, 0	},
	{	0xFFFFFA09L, eCOCOON_SIZE_BYTE, 0, 0	},
	{	0xFFFFFA0BL, eCOCOON_SIZE_BYTE, 0, 0	},
	{	0xFFFFFA0DL, eCOCOON_SIZE_BYTE, 0, 0	},
	{	0xFFFFFA0FL, eCOCOON_SIZE_BYTE, 0, 0	},
	{	0xFFFFFA11L, eCOCOON_SIZE_BYTE, 0, 0	},
	{	0xFFFFFA13L, eCOCOON_SIZE_BYTE, 0, 0	},
	{	0xFFFFFA15L, eCOCOON_SIZE_BYTE, 0, 0	},
	{	0xFFFFFA17L, eCOCOON_SIZE_BYTE, 0, 0	},
	{	0xFFFFFA19L, eCOCOON_SIZE_BYTE, 0, 0	},
	{	0xFFFFFA1BL, eCOCOON_SIZE_BYTE, 0, 0	},
	{	0xFFFFFA1DL, eCOCOON_SIZE_BYTE, 0, 0	},
	{	0xFFFFFA1FL, eCOCOON_SIZE_BYTE, 0, 0	},
	{	0xFFFFFA21L, eCOCOON_SIZE_BYTE, 0, 0	},
	{	0xFFFFFA23L, eCOCOON_SIZE_BYTE, 0, 0	},
	{	0xFFFFFA25L, eCOCOON_SIZE_BYTE, 0, 0	},
	{	0xFFFFFA27L, eCOCOON_SIZE_BYTE, 0, 0	},
	{	0xFFFFFA29L, eCOCOON_SIZE_BYTE, 0, 0	},
	{	0xFFFFFA2BL, eCOCOON_SIZE_BYTE, 0, 0	},
	{	0xFFFFFA2DL, eCOCOON_SIZE_BYTE, 0, 0	},
	{	0xFFFFFA2FL, eCOCOON_SIZE_BYTE, 0, 0	},
};

sCocoonMarker	gCocoonShifter[ eCOCOON_SHIFTER_LIMIT ] =
{
	{	0xFFFF8201L, eCOCOON_SIZE_BYTE, 0, 0	},
	{	0xFFFF8203L, eCOCOON_SIZE_BYTE, 0, 0	},
	{	0xFFFF820AL, eCOCOON_SIZE_BYTE, 0, 0	},
	{	0xFFFF8260L, eCOCOON_SIZE_BYTE, 0, 0	}
};

sCocoonMarker	gCocoonShifterSTE[ eCOCOON_SHIFTERSTE_LIMIT ] =
{
	{	0xFFFF8201L, eCOCOON_SIZE_BYTE, 0, 0	},
	{	0xFFFF8203L, eCOCOON_SIZE_BYTE, 0, 0	},
	{	0xFFFF820AL, eCOCOON_SIZE_BYTE, 0, 0	},
	{	0xFFFF820DL, eCOCOON_SIZE_BYTE, 0, 0	},
	{	0xFFFF820FL, eCOCOON_SIZE_BYTE, 0, 0	},
	{	0xFFFF8260L, eCOCOON_SIZE_BYTE, 0, 0	},
	{	0xFFFF8265L, eCOCOON_SIZE_BYTE, 0, 0	}
};

sCocoonMarker	gCocoonVidel[ eCOCOON_VIDEL_LIMIT ] =
{
	{	0xFFFF8006L, eCOCOON_SIZE_BYTE,	0	},
	{	0xFFFF8201L, eCOCOON_SIZE_BYTE,	0	}, 
	{	0xFFFF8203L, eCOCOON_SIZE_BYTE,	0	},
	{	0xFFFF820DL, eCOCOON_SIZE_BYTE,	0	},
	{	0xFFFF820EL, eCOCOON_SIZE_WORD,	0	},
	{	0xFFFF8210L, eCOCOON_SIZE_WORD,	0	},
	{	0xFFFF8260L, eCOCOON_SIZE_BYTE,	0	},
	{	0xFFFF8265L, eCOCOON_SIZE_WORD,	0	},
	{	0xFFFF8266L, eCOCOON_SIZE_WORD,	0	},
	{	0xFFFF8280L, eCOCOON_SIZE_WORD,	0	},
	{	0xFFFF8282L, eCOCOON_SIZE_WORD,	0	},
	{	0xFFFF8284L, eCOCOON_SIZE_WORD,	0	},
	{	0xFFFF8286L, eCOCOON_SIZE_WORD,	0	},
	{	0xFFFF8288L, eCOCOON_SIZE_WORD,	0	},
	{	0xFFFF828AL, eCOCOON_SIZE_WORD,	0	},
	{	0xFFFF828CL, eCOCOON_SIZE_WORD,	0	},
	{	0xFFFF8290L, eCOCOON_SIZE_WORD,	0	},
	{	0xFFFF82A0L, eCOCOON_SIZE_WORD,	0	},
	{	0xFFFF82A2L, eCOCOON_SIZE_WORD,	0	},
	{	0xFFFF82A4L, eCOCOON_SIZE_WORD,	0	},
	{	0xFFFF82A6L, eCOCOON_SIZE_WORD,	0	},
	{	0xFFFF82A8L, eCOCOON_SIZE_WORD,	0	},
	{	0xFFFF82AAL, eCOCOON_SIZE_WORD,	0	},
	{	0xFFFF82ACL, eCOCOON_SIZE_WORD,	0	},
	{	0xFFFF82C0L, eCOCOON_SIZE_WORD,	0	},
	{	0xFFFF82C2L, eCOCOON_SIZE_WORD,	0	}
};

sCocoonMarker	gCocoonDmaSoundFalcon[ eCOCOON_DMASOUNDFALCON_LIMIT ] =
{
	{	0xFFFF8900L, eCOCOON_SIZE_BYTE, 0, 0	},
	{	0xFFFF8901L, eCOCOON_SIZE_BYTE, 0, 0	},
	{	0xFFFF8920L, eCOCOON_SIZE_BYTE, 0, 0	},
	{	0xFFFF8921L, eCOCOON_SIZE_BYTE, 0, 0	},
};

sCocoonMarker	gCocoonDmaSoundSte[ eCOCOON_DMASOUNDSTE_LIMIT ] =
{
	{	0xFFFF8901L, eCOCOON_SIZE_BYTE, 0, 0	},
	{	0xFFFF8921L, eCOCOON_SIZE_BYTE, 0, 0	},
};

sCocoonMarker	gCocoonCrossbar[ eCOCOON_CROSSBAR_LIMIT ] =
{
	{	eCOCOON_CROSSBAR_FFFF8930,	eCOCOON_SIZE_WORD, 0,	0	},
	{	eCOCOON_CROSSBAR_FFFF8932,	eCOCOON_SIZE_WORD, 0,	0	},
	{	eCOCOON_CROSSBAR_FFFF8934,	eCOCOON_SIZE_BYTE, 0,	0	},
	{	eCOCOON_CROSSBAR_FFFF8935,	eCOCOON_SIZE_BYTE, 0,	0	},
	{	eCOCOON_CROSSBAR_FFFF8936,	eCOCOON_SIZE_BYTE, 0,	0	},
	{	eCOCOON_CROSSBAR_FFFF8937,	eCOCOON_SIZE_BYTE, 0,	0	},
	{	eCOCOON_CROSSBAR_FFFF8938,	eCOCOON_SIZE_BYTE, 0,	0	},
	{	eCOCOON_CROSSBAR_FFFF8939,	eCOCOON_SIZE_BYTE, 0,	0	},
	{	eCOCOON_CROSSBAR_FFFF893A,	eCOCOON_SIZE_BYTE, 0,	0	},
	{	eCOCOON_CROSSBAR_FFFF893C,	eCOCOON_SIZE_BYTE, 0,	0	},
	{	eCOCOON_CROSSBAR_FFFF8941,	eCOCOON_SIZE_BYTE, 0,	0	},
	{	eCOCOON_CROSSBAR_FFFF8943,	eCOCOON_SIZE_BYTE, 0,	0	}
};

U8						gCocoonYM[ dCOCOON_YM_LIMIT ];
U8						gCocoonYMNew[ dCOCOON_YM_LIMIT ];

char					gNull[ 2 ]	= { 0, 0 };
sCocoon					gCocoon;
sFileHandle				gCocoonLogFileHandle;
sCocoonMarkerHeader		gCocoonHardWare[ eCOCOON_HW_LIMIT ];

/* ###################################################################################
#  PROTOTYPES
################################################################################### */

void	Cocoon_Init( void );

void	Cocoon_SystemVecsStore( void );
void	Cocoon_SystemVarsStore( void );
void	Cocoon_HardWareStore( void );
void	Cocoon_HardWareYMStore( void );
void	Cocoon_Store( void );

void	Cocoon_SystemVecsCheck( void );
void	Cocoon_SystemVarsCheck( void );
void	Cocoon_HardWareCheck( void );
void	Cocoon_HardWareYMCheck( void );
void	Cocoon_Check( void );

void	Cocoon_SystemVecsLogChanges( void );
void	Cocoon_SystemVarsLogChanges( void );
void	Cocoon_HardWareLogChanges( void );
void	Cocoon_HardWareYMLogChanges( void );
S32		Cocoon_LogChanges( void );

void	Cocoon_LogFileOpen( void );
void	Cocoon_LogFileWrite( char * apString );
void	Cocoon_LogFileClose( void );


/* ###################################################################################
#  CODE
################################################################################### */

S16	main( S16 argc, char * argv[] )
{
	char			lDirectory[ 512 ];
	char *			lpFileName;
	S32				lSize;

	GemDos_Super( 0 );

	Cocoon_Init();

	printf( "< COCOON >\n\n" );
	if( argc < 2 )
	{
		printf( "usage : COCOON.TTP <programfile>\n" );
		GemDos_Cnecin();
	}
	lpFileName = argv[ 1 ];
/*	lpFileName = "C:\\SOURCE\\RG\\CHUCHU\\CHUCHU.TOS";*/
/*	lpFileName = "I:\\CHUCHU\\CHUCHU.TOS";*/

	lSize = strlen( lpFileName );
	if( lSize )
	{
		lSize--;
		while( (lSize) && (lpFileName[lSize] != '\\') )
		{
			lSize--;
		}
		while( lSize >= 0 )
		{
			lDirectory[ lSize ] = lpFileName[ lSize ];
			lSize--;
		}
	}

	printf( "loading : %s\n", lpFileName );

	Drive_SetPath( lDirectory );
	Xbios_Supexec( Cocoon_Store );
	GemDos_Pexec( 0, lpFileName, gNull, gNull );
	Xbios_Supexec( Cocoon_Check );
	Cocoon_LogFileOpen();
	Cocoon_LogChanges();
	Cocoon_LogFileClose();

	return( 1 );
}

void	Cocoon_Init( void )
{
	gCocoonHardWare[ eCOCOON_HW_BUSCONTROL ].Count        = eCOCOON_BUSCONTROL_LIMIT;
	gCocoonHardWare[ eCOCOON_HW_BUSCONTROL ].MachineTypes = eCOCOON_MCH_ST | eCOCOON_MCH_STE | eCOCOON_MCH_TT | eCOCOON_MCH_FALCON;
	gCocoonHardWare[ eCOCOON_HW_BUSCONTROL ].pMarker      = gCocoonBusControl;

	gCocoonHardWare[ eCOCOON_HW_MMU ].Count        = eCOCOON_MMU_LIMIT;
	gCocoonHardWare[ eCOCOON_HW_MMU ].MachineTypes = eCOCOON_MCH_ST | eCOCOON_MCH_STE | eCOCOON_MCH_TT | eCOCOON_MCH_FALCON;
	gCocoonHardWare[ eCOCOON_HW_MMU ].pMarker      = gCocoonMMU;

	gCocoonHardWare[ eCOCOON_HW_SHIFTER ].Count        = eCOCOON_SHIFTER_LIMIT;
	gCocoonHardWare[ eCOCOON_HW_SHIFTER ].MachineTypes = eCOCOON_MCH_ST;
	gCocoonHardWare[ eCOCOON_HW_SHIFTER ].pMarker      = gCocoonShifter;

	gCocoonHardWare[ eCOCOON_HW_SHIFTERSTE ].Count        = eCOCOON_SHIFTERSTE_LIMIT;
	gCocoonHardWare[ eCOCOON_HW_SHIFTERSTE ].MachineTypes = eCOCOON_MCH_STE;
	gCocoonHardWare[ eCOCOON_HW_SHIFTERSTE ].pMarker      = gCocoonShifterSTE;

	gCocoonHardWare[ eCOCOON_HW_MFP ].Count        = eCOCOON_MFP_LIMIT;
	gCocoonHardWare[ eCOCOON_HW_MFP ].MachineTypes = eCOCOON_MCH_ST | eCOCOON_MCH_STE | eCOCOON_MCH_TT | eCOCOON_MCH_FALCON;
	gCocoonHardWare[ eCOCOON_HW_MFP ].pMarker      = gCocoonMFP;

	gCocoonHardWare[ eCOCOON_HW_VIDEL ].Count        = eCOCOON_VIDEL_LIMIT;
	gCocoonHardWare[ eCOCOON_HW_VIDEL ].MachineTypes = eCOCOON_MCH_FALCON;
	gCocoonHardWare[ eCOCOON_HW_VIDEL ].pMarker      = gCocoonVidel;

	gCocoonHardWare[ eCOCOON_HW_DMASOUNDFALCON ].Count        = eCOCOON_DMASOUNDFALCON_LIMIT;
	gCocoonHardWare[ eCOCOON_HW_DMASOUNDFALCON ].MachineTypes = eCOCOON_MCH_FALCON;
	gCocoonHardWare[ eCOCOON_HW_DMASOUNDFALCON ].pMarker      = gCocoonDmaSoundFalcon;

	gCocoonHardWare[ eCOCOON_HW_DMASOUNDSTE ].Count        = eCOCOON_DMASOUNDSTE_LIMIT;
	gCocoonHardWare[ eCOCOON_HW_DMASOUNDSTE ].MachineTypes = eCOCOON_MCH_STE;
	gCocoonHardWare[ eCOCOON_HW_DMASOUNDSTE ].pMarker      = gCocoonDmaSoundSte;

	gCocoonHardWare[ eCOCOON_HW_CROSSBAR ].Count        = eCOCOON_CROSSBAR_LIMIT;
	gCocoonHardWare[ eCOCOON_HW_CROSSBAR ].MachineTypes = eCOCOON_MCH_FALCON;
	gCocoonHardWare[ eCOCOON_HW_CROSSBAR ].pMarker      = gCocoonCrossbar;
}

void	Cocoon_SystemVecsStore( void )
{
	U32 *	lpVecs;
	U16		i;

	lpVecs = (U32*)dCOCOON_SYSVEC_STARTADR;

	for( i=0; i<dCOCOON_SYSVEC_LIMIT; i++ )
	{
		gCocoon.SysVecs[ i ] = lpVecs[ i ];
	}
}


void	Cocoon_SystemVarsStore( void )
{
	U16 *	lpVars;
	U16		i;

	lpVars = (U16*)dCOCOON_SYSVAR_STARTADR;

	for( i=0; i<dCOCOON_SYSVAR_LIMIT; i++ )
	{
		gCocoon.SysVars[ i ] = lpVars[ i ];
	}
}

void	Cocoon_HardWareStore( void )
{
	U16		i,j;
	U32		lMachine;
	U8 *	lpByte;
	U16 *	lpWord;
	U32 *	lpLong;

	System_CalcInfo();

	lMachine = System_GetMCH();
	lMachine = 1L<<lMachine;

	for( i=0; i<eCOCOON_HW_LIMIT; i++ )
	{
		if( gCocoonHardWare[ i ].pMarker )
		{
			if( gCocoonHardWare[ i ].MachineTypes & lMachine )
			{
				for( j=0; j<gCocoonHardWare[ i ].Count; j++ )
				{
					switch( (U16)gCocoonHardWare[ i ].pMarker[ j ].Size )
					{
					case	eCOCOON_SIZE_BYTE:
						lpByte = (U8*)gCocoonHardWare[ i ].pMarker[ j ].Address;
						gCocoonHardWare[ i ].pMarker[ j ].Value = *lpByte;
						break;
					case	eCOCOON_SIZE_WORD:
						lpWord = (U16*)gCocoonHardWare[ i ].pMarker[ j ].Address;
						gCocoonHardWare[ i ].pMarker[ j ].Value = *lpWord;
						break;
					case	eCOCOON_SIZE_LONG:
						lpLong = (U32*)gCocoonHardWare[ i ].pMarker[ j ].Address;
						gCocoonHardWare[ i ].pMarker[ j ].Value = *lpLong;
						break;
					}
				}
			}
		}
	}
}

void	Cocoon_HardWareYMStore( void )
{
	U8					i;
	volatile	U8 *	lpYM;

	lpYM = (U8*)0xFFFF8800L;

	for( i=0; i<dCOCOON_YM_LIMIT; i++ )
	{
		*lpYM = i;
		gCocoonYM[ i ] = *lpYM;
	}
}

void	Cocoon_Store( void )
{
	Cocoon_SystemVecsStore();
	Cocoon_SystemVarsStore();
	Cocoon_HardWareStore();
	Cocoon_HardWareYMStore();
}

void	Cocoon_SystemVecsCheck( void )
{
	U32 *	lpVecs;
	U16		i;

	lpVecs = (U32*)dCOCOON_SYSVEC_STARTADR;

	for( i=0; i<dCOCOON_SYSVEC_LIMIT; i++ )
	{
		gCocoon.SysVecsNew[ i ] = lpVecs[ i ];
	}
}

void	Cocoon_SystemVarsCheck( void )
{
	U16 *	lpVars;
	U16		i;

	lpVars = (U16*)dCOCOON_SYSVAR_STARTADR;

	for( i=0; i<dCOCOON_SYSVAR_LIMIT; i++ )
	{
		gCocoon.SysVarsNew[ i ] = lpVars[ i ];
	}
}

void	Cocoon_HardWareCheck( void )
{
	U16		i,j;
	U32		lMachine;
	U8 *	lpByte;
	U16 *	lpWord;
	U32 *	lpLong;

	System_CalcInfo();

	lMachine = System_GetMCH();
	lMachine = 1L<<lMachine;

	for( i=0; i<eCOCOON_HW_LIMIT; i++ )
	{
		if( gCocoonHardWare[ i ].pMarker )
		{
			if( gCocoonHardWare[ i ].MachineTypes & lMachine )
			{
				for( j=0; j<gCocoonHardWare[ i ].Count; j++ )
				{
					switch( (U16)gCocoonHardWare[ i ].pMarker[ j ].Size )
					{
					case	eCOCOON_SIZE_BYTE:
						lpByte = (U8*)gCocoonHardWare[ i ].pMarker[ j ].Address;
						gCocoonHardWare[ i ].pMarker[ j ].NewValue = *lpByte;
						break;
					case	eCOCOON_SIZE_WORD:
						lpWord = (U16*)gCocoonHardWare[ i ].pMarker[ j ].Address;
						gCocoonHardWare[ i ].pMarker[ j ].NewValue = *lpWord;
						break;
					case	eCOCOON_SIZE_LONG:
						lpLong = (U32*)gCocoonHardWare[ i ].pMarker[ j ].Address;
						gCocoonHardWare[ i ].pMarker[ j ].NewValue = *lpLong;
						break;
					}
				}
			}
		}
	}
}

void	Cocoon_HardWareYMCheck( void )
{
	U8					i;
	volatile	U8 *	lpYM;

	lpYM = (U8*)0xFFFF8800L;

	for( i=0; i<dCOCOON_YM_LIMIT; i++ )
	{
		*lpYM              = i;
		gCocoonYMNew[ i ] = *lpYM;
	}
}

void	Cocoon_Check( void )
{
	Cocoon_SystemVecsCheck();
	Cocoon_SystemVarsCheck();	
	Cocoon_HardWareCheck();
	Cocoon_HardWareYMCheck();
}

void	Cocoon_SystemVecsLogChanges( void )
{
	char	lString[ 128 ];
	U32 *	lpVecs;
	U16		i;

	lpVecs = (U32*)dCOCOON_SYSVEC_STARTADR;

	for( i=0; i<dCOCOON_SYSVEC_LIMIT; i++ )
	{
		if( gCocoon.SysVecs[ i ] != gCocoon.SysVecsNew[ i ] )
		{
			sprintf( lString, "%p\t%lX\t%lX\n", &lpVecs[i], gCocoon.SysVecs[i], gCocoon.SysVecsNew[i] );
			Cocoon_LogFileWrite( lString );
		}
	}
}

void	Cocoon_SystemVarsLogChanges( void )
{
	char	lString[ 128 ];
	U16 *	lpVars;
	U16		i;

	lpVars = (U16*)dCOCOON_SYSVAR_STARTADR;

	for( i=0; i<dCOCOON_SYSVAR_LIMIT; i++ )
	{
		if( gCocoon.SysVars[ i ] != gCocoon.SysVarsNew[ i ] )
		{
			sprintf( lString, "%p\t%X\t%X\n", &lpVars[i], gCocoon.SysVars[i], gCocoon.SysVarsNew[i] );
			Cocoon_LogFileWrite( lString );
		}
	}
}

void	Cocoon_HardWareLogChanges( void )
{
	char	lString[ 128 ];
	U16		i,j;
	U32		lMachine;

	System_CalcInfo();

	lMachine = System_GetMCH();
	lMachine = 1L<<lMachine;

	for( i=0; i<eCOCOON_HW_LIMIT; i++ )
	{
		if( gCocoonHardWare[ i ].pMarker )
		{
			if( gCocoonHardWare[ i ].MachineTypes & lMachine )
			{
				for( j=0; j<gCocoonHardWare[ i ].Count; j++ )
				{
					switch( (U16)gCocoonHardWare[ i ].pMarker[ j ].Size )
					{
					case	eCOCOON_SIZE_BYTE:
						if( gCocoonHardWare[ i ].pMarker[ j ].Value != gCocoonHardWare[ i ].pMarker[ j ].NewValue )
						{
							sprintf( lString, "%lX\t%lX\t%X\n", gCocoonHardWare[ i ].pMarker[ j ].Address, gCocoonHardWare[ i ].pMarker[ j ].Value, gCocoonHardWare[ i ].pMarker[ j ].NewValue );
							Cocoon_LogFileWrite( lString );
						}
						break;
					case	eCOCOON_SIZE_WORD:
						if( gCocoonHardWare[ i ].pMarker[ j ].Value != gCocoonHardWare[ i ].pMarker[ j ].NewValue )
						{
							sprintf( lString, "%lX\t%lX\t%X\n", gCocoonHardWare[ i ].pMarker[ j ].Address, gCocoonHardWare[ i ].pMarker[ j ].Value, gCocoonHardWare[ i ].pMarker[ j ].NewValue );
							Cocoon_LogFileWrite( lString );
						}
						break;
					case	eCOCOON_SIZE_LONG:
						if( gCocoonHardWare[ i ].pMarker[ j ].Value != gCocoonHardWare[ i ].pMarker[ j ].NewValue )
						{
							sprintf( lString, "%lX\t%lX\t%lX\n", gCocoonHardWare[ i ].pMarker[ j ].Address, gCocoonHardWare[ i ].pMarker[ j ].Value, gCocoonHardWare[ i ].pMarker[ j ].NewValue );
							Cocoon_LogFileWrite( lString );
						}
						break;
					}
				}
			}
		}
	}
}

void	Cocoon_HardWareYMLogChanges( void )
{
	char				lString[ 128 ];
	U16					i;


	for( i=0; i<dCOCOON_YM_LIMIT; i++ )
	{
		if( gCocoonYM[ i ] != gCocoonYMNew[ i ] )
		{
			sprintf( lString, "FFFF8800:%X\t%X\t%X\n", i, gCocoonYM[i], gCocoonYMNew[i] );
			Cocoon_LogFileWrite( lString );
		}
	}
}


S32		Cocoon_LogChanges( void )
{
	Cocoon_SystemVecsLogChanges();
	Cocoon_SystemVarsLogChanges();
	Cocoon_HardWareLogChanges();
	Cocoon_HardWareYMLogChanges();
	return( 0 );
}

void	Cocoon_LogFileOpen( void )
{
	gCocoonLogFileHandle = File_Create( "COCOON.LOG" );
}

void	Cocoon_LogFileWrite( char * apString )
{
	U32	lSize;

	if( apString )
	{
		lSize = strlen( apString );
		if( lSize )
		{
			File_Write( gCocoonLogFileHandle, lSize, apString );
		}
	}
}

void	Cocoon_LogFileClose( void )
{
	File_Close( gCocoonLogFileHandle );
}
