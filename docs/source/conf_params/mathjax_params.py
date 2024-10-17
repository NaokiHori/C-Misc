mathjax_path = "https://cdn.jsdelivr.net/npm/mathjax@2/MathJax.js?config=TeX-AMS-MML_HTMLorMML"

macros = dict()

macros["seq"] = ["#1 = 0, 1, \\cdots, #2", 2]

macros["twiddle"] = ["{\\exp \\left( {#1} \\frac{#2}{#3} I \\right)}", 3]
macros["ctwiddle"] = ["{\\cos \\left( {#1} \\frac{#2}{#3} \\right)}", 3]
macros["stwiddle"] = ["{\\sin \\left( {#1} \\frac{#2}{#3} \\right)}", 3]

macros["rdft"] = ["\\mathcal{F}_{#1} \\left[ #2_{#3}, #2_{#4}, \\cdots, #2_{#5} \\right]", 5]
macros["irdft"] = ["\\mathcal{F}_{#1}^{-1} \\left[ #2_{#3}, #2_{#4}, \\cdots, #2_{#5} \\right]", 5]

macros["dct"] = ["\\mathcal{C}_{#2} \\left[ {#1}; {#3}, {#4}, \\cdots, {#5} \\right]", 5]
macros["idct"] = ["\\mathcal{C}_{#2}^{-1} \\left[ {#1}; {#3}, {#4}, \\cdots, {#5} \\right]", 5]

mathjax3_config = {"TeX": {"Macros": macros}}
