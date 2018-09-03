PUSHD
CHDIR "..\..\steem\asm"
%NASMROOT%\nasm -fwin32 -dWIN32 -d_VC_BUILD -oasm_draw_VC.obj -w+macro-params -w+macro-selfref -w+orphan-labels asm_draw.asm
%NASMROOT%\nasm -fwin32 -oasm_osd_VC.obj -dWIN32 -d_VC_BUILD -w+macro-params -w+macro-selfref -w+orphan-labels asm_osd_draw.asm
POPD
echo "asm done"
