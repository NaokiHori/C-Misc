# Real-Valued Fast Fourier Transform (no complex)

Same as the normal one, but the input / output signals are pure complex, which is achieved by ordering the complex signal as 

`Re(0)`, `Re(1)`, ..., `Re(N / 2 - 1)`, `Re(N / 2)`, `Im(N / 2 - 1)`, `Im(N / 2 - 2)`, ..., `Im(2)`, `Im(1)`.

