Write-Host "Compiling entire project..." -ForegroundColor "Green"
$buildDirectory = "qca/build"
$installDirectory = "qca/install"
$rootDirectory = $PWD
Remove-Item -Recurse -Force $buildDirectory -ErrorAction Ignore | Out-Null
Remove-Item -Recurse -Force $installDirectory -ErrorAction Ignore | Out-Null
New-Item -Path ${PWD} -Name $buildDirectory -ItemType "directory" | Out-Null
New-Item -Path ${PWD} -Name $installDirectory -ItemType "directory" | Out-Null
Push-Location $buildDirectory
cmake -DCMAKE_TOOLCHAIN_FILE="D:\vcpkg\scripts\buildsystems\vcpkg.cmake" -DCMAKE_INSTALL_PREFIX="${PWD}/${installDirectory}" ../../
cmake --build . --config Release --parallel 24 && cmake --install . --config Release
cmake --build . --config Debug --parallel 24 && cmake --install . --config Debug
Pop-Location
Write-Host "Cleaning up..." -ForegroundColor "Green"
Remove-Item -Recurse -Force $buildDirectory -ErrorAction Ignore | Out-Null
Remove-Item -Recurse -Force $installDirectory -ErrorAction Ignore | Out-Null

Write-Host "Compiling separate project..." -ForegroundColor "Green"
Remove-Item -Recurse -Force $installDirectory -ErrorAction Ignore | Out-Null

foreach ($item in @("core", "security", "data", "widgets"))
{
    Write-Host "   Qtz ${item}..." -ForegroundColor "Green"
    Remove-Item -Recurse -Force $buildDirectory -ErrorAction Ignore | Out-Null
    New-Item -Path ${PWD} -Name $buildDirectory -ItemType "directory" | Out-Null
    Push-Location $buildDirectory
    cmake -DCMAKE_TOOLCHAIN_FILE="D:\vcpkg\scripts\buildsystems\vcpkg.cmake" -DCMAKE_PREFIX_PATH="${rootDirectory}/${installDirectory}/lib/cmake/" -DCMAKE_INSTALL_PREFIX="${rootDirectory}/${installDirectory}" ../../${item}/
    cmake --build . --config Release --parallel 24 && cmake --install . --config Release
    cmake --build . --config Debug --parallel 24 && cmake --install . --config Debug
    Pop-Location
}
Write-Host "Cleaning up..." -ForegroundColor "Green"
Remove-Item -Recurse -Force $buildDirectory -ErrorAction Ignore | Out-Null






