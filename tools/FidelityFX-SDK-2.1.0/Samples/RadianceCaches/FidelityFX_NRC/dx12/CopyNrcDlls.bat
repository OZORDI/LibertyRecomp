:: This file is part of the FidelityFX SDK.
:: 
:: Copyright (C) 2025 Advanced Micro Devices, Inc.
:: 
:: Permission is hereby granted, free of charge, to any person obtaining a copy
:: of this software and associated documentation files(the "Software"), to deal
:: in the Software without restriction, including without limitation the rights
:: to use, copy, modify, merge, publish, distribute, sublicense, and /or sell
:: copies of the Software, and to permit persons to whom the Software is
:: furnished to do so, subject to the following conditions :
:: 
:: The above copyright notice and this permission notice shall be included in
:: all copies or substantial portions of the Software.
:: 
:: THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
:: IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
:: FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
:: AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
:: LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
:: OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
:: THE SOFTWARE.

@echo off

set "SOLUTION_DIR=%~1"
set "AMDINTERNAL_DIR=%SOLUTION_DIR%..\..\..\..\Kits\FidelityFX\amdinternal"

echo Copying NRC DLLs and PDBs...
echo %AMDINTERNAL_DIR%

set "SRC_DLL=%AMDINTERNAL_DIR%\radiancecache\dx12\x64\Debug\amd_fidelityfx_radiancecache_dx12d.dll"
set "DST_DLL=C:\GitHubEMU\FidelityFX-Samples\Samples\RadianceCaches\FidelityFX_NRC\dx12\x64\Debug\amd_fidelityfx_radiancecache_dx12d.dll"

echo Source DLL:
for %%F in ("%SRC_DLL%") do (
    echo Full path : %%~fF
    echo Modified  : %%~tF
    echo Size      : %%~zF bytes
)

echo Destination DLL:
for %%F in ("%DST_DLL%") do (
    echo Full path : %%~fF
    echo Modified  : %%~tF
    echo Size      : %%~zF bytes
)

xcopy %SRC_DLL% %DST_DLL% /Y /Q /I

set "SRC_PDB=%AMDINTERNAL_DIR%\amdinternal\radiancecache\dx12\x64\Debug\amd_fidelityfx_radiancecache_dx12d.pdb"
set "DST_PDB=C:\GitHubEMU\FidelityFX-Samples\Samples\RadianceCaches\FidelityFX_NRC\dx12\x64\Debug"
xcopy %SRC_PDB% %DST_PDB% /Y /Q /I