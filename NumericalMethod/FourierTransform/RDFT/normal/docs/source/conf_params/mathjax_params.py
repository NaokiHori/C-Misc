mathjax_path = "https://cdn.jsdelivr.net/npm/mathjax@2/MathJax.js?config=TeX-AMS-MML_HTMLorMML"

macros = dict()

macros["twiddle"]  = ["\\exp \\left( #1 2 \\pi \\frac{#2}{#3} I \\right)", 3]
macros["seq"]      = ["#1 = 0, 1, \\cdots, #2", 2]
macros["fourier"]  = ["\\mathcal{F}_{#1} \\left[ #2_{#3}, #2_{#4}, \\cdots, #2_{#5} \\right]", 5]
macros["ifourier"] = ["\\mathcal{F}_{#1}^{-1} \\left[ #2_{#3}, #2_{#4}, \\cdots, #2_{#5} \\right]", 5]

mathjax3_config = {"TeX": {"Macros": macros}}
