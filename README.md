# NMRI
2D Fourier Transform of Nuclear Magnetic Resonance Imaging raw data, 3D visualization with VTK

[![Codacy Badge](https://api.codacy.com/project/badge/Grade/9249fa3267d84c819fc48af840b73392)](https://app.codacy.com/gh/aromanro/NMRI?utm_source=github.com&utm_medium=referral&utm_content=aromanro/NMRI&utm_campaign=Badge_Grade_Settings)
[![CodeFactor](https://www.codefactor.io/repository/github/aromanro/nmri/badge)](https://www.codefactor.io/repository/github/aromanro/nmri)

Description on https://compphys.go.ro/nuclear-magnetic-resonance-and-fourier-transform/

Basically this is a quickly implemented project to test some wrapper classes for FFTW library: http://www.fftw.org/ Obviously, the project needs the FFTW library.
A later addition was the 3D visualization, using VTK: https://vtk.org/

It's a 2D Fourier Transform of Nuclear Magnetic Resonance Imaging raw data. It displays both the raw data and the image, allows cutting out low and high frequency information.
It also has a 3D view implemented with VTK.

It needs the Head2D.dat raw data file one can find here: http://download.nvidia.com/developer/GPU_Gems_2/CD/Index.html in the source tree from Chap. 48, Medical Image Reconstruction with the FFT.
By the way, here is the chapter, in case somebody wants to look over it: https://developer.nvidia.com/gpugems/GPUGems2/gpugems2_chapter48.html

### PROGRAM IN ACTION

[![Program video](https://img.youtube.com/vi/toGlT4gNKds/0.jpg)](https://youtu.be/toGlT4gNKds)
