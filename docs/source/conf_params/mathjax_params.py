mathjax_path = "https://cdn.jsdelivr.net/npm/mathjax@2/MathJax.js?config=TeX-AMS-MML_HTMLorMML"

macros = dict()

macros["seq"] = ["#1 = 0, 1, \\cdots, #2", 2]

macros["twiddle"]  = ["{\\exp \\left( {#1} \\frac{#2}{#3} I \\right)}", 3]
macros["ctwiddle"] = ["{\\cos \\left( {#1} \\frac{#2}{#3}   \\right)}", 3]
macros["stwiddle"] = ["{\\sin \\left( {#1} \\frac{#2}{#3}   \\right)}", 3]

macros["dft"]    = ["\\mathcal{F}_{#1}      \\left[ {#2}; {#3}, {#4}, \\cdots, {#5} \\right]", 5]
macros["idft"]   = ["\\mathcal{F}_{#1}^{-1} \\left[ {#2}; {#3}, {#4}, \\cdots, {#5} \\right]", 5]

macros["dctii"]  = ["\\mathcal{C}_{#1}      \\left[ {#2}; {#3}, {#4}, \\cdots, {#5} \\right]", 5]
macros["dctiii"] = ["\\mathcal{C}_{#1}^{-1} \\left[ {#2}; {#3}, {#4}, \\cdots, {#5} \\right]", 5]

mathjax3_config = {"TeX": {"Macros": macros}}
