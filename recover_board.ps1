# =============================================================================
# Sentinel-RT Board Recovery Script
# Run this script AFTER:
#   1. BOOT0 switch is set to position "1" (flipped from normal)
#   2. BOTH USB cables are plugged in (ST-LINK top + USB-C bottom)
#   3. Board has been reset (press NRST button)
# =============================================================================

$CLI  = "C:\ST\STM32CubeIDE_2.2.0\STM32CubeIDE\plugins\com.st.stm32cube.ide.mcu.externaltools.cubeprogrammer.win32_2.2.500.202603051304\tools\bin\STM32_Programmer_CLI.exe"
$EXT  = "C:\ST\STM32CubeIDE_2.2.0\STM32CubeIDE\plugins\com.st.stm32cube.ide.mcu.externaltools.cubeprogrammer.win32_2.2.500.202603051304\tools\bin\ExternalLoader\MX66UW1G45G_STM32N6570-DK.stldr"
$FSBL = "C:\Users\user\Documents\Sentinel-RT\STM_MX_till_now\FSBL\Debug\STM_MX_till_now_FSBL.bin"

Write-Host "=============================================" -ForegroundColor Cyan
Write-Host " Sentinel-RT Board Recovery" -ForegroundColor Cyan
Write-Host "=============================================" -ForegroundColor Cyan

# --- Step 1: Check DFU device is visible ---
Write-Host "`nStep 1: Checking for DFU device..." -ForegroundColor Yellow
$dfuCheck = & $CLI -l usb 2>&1
if ($dfuCheck -match "DFU") {
    Write-Host "  DFU device found!" -ForegroundColor Green
} else {
    Write-Host "  ERROR: No DFU device found." -ForegroundColor Red
    Write-Host "  Make sure BOOT0 is switched to '1' and NRST was pressed." -ForegroundColor Red
    Write-Host "  Also ensure the USB-C cable (bottom port) is connected." -ForegroundColor Red
    exit 1
}

# --- Step 2: Write FSBL.bin to XSPI flash at 0x70000000 via DFU ---
Write-Host "`nStep 2: Writing FSBL to XSPI flash..." -ForegroundColor Yellow
& $CLI -c port=usb1 -el $EXT -d $FSBL 0x70000000
if ($LASTEXITCODE -ne 0) {
    Write-Host "  ERROR: Failed to write FSBL. Trying SWD fallback..." -ForegroundColor Red
    # Fallback: try via SWD ST-LINK
    & $CLI -c port=SWD mode=UR -el $EXT -d $FSBL 0x70000000
    if ($LASTEXITCODE -ne 0) {
        Write-Host "  Both methods failed. Please check USB connections." -ForegroundColor Red
        exit 1
    }
}
Write-Host "  FSBL written to XSPI flash!" -ForegroundColor Green

# --- Step 3: Instruct user to flip BOOT0 back and reset ---
Write-Host "`n=============================================" -ForegroundColor Cyan
Write-Host " RECOVERY COMPLETE!" -ForegroundColor Green
Write-Host "=============================================" -ForegroundColor Cyan
Write-Host "`nNow do this:" -ForegroundColor Yellow
Write-Host "  1. Slide the BOOT0 switch BACK to its original position" -ForegroundColor White
Write-Host "  2. Press the NRST button to reset the board" -ForegroundColor White
Write-Host "  3. The board will boot normally again" -ForegroundColor White
Write-Host "  4. Open STM32CubeIDE and click the Green Bug to debug" -ForegroundColor White
