mathjax_path = "https://cdn.jsdelivr.net/npm/mathjax@2/MathJax.js?config=TeX-AMS-MML_HTMLorMML"

macros = dict()

macros["real"] = ["{\\Re \\left[ #1 \\right]}", 1]
macros["imag"] = ["{\\Im \\left[ #1 \\right]}", 1]

macros["seq"] = ["{{#1} = {#2}, {#3}, \\cdots, {#4}}", 4]

macros["twiddle"]  = ["{\\exp \\left( {#1} \\frac{#2}{#3} I \\right)}", 3]
macros["ctwiddle"] = ["{\\cos \\left( {#1} \\frac{#2}{#3}   \\right)}", 3]
macros["stwiddle"] = ["{\\sin \\left( {#1} \\frac{#2}{#3}   \\right)}", 3]

macros["dft"]    = ["\\mathcal{F}_{#1}      \\left[ {#2}; {#3}, {#4}, \\cdots, {#5} \\right]", 5]
macros["idft"]   = ["\\mathcal{F}_{#1}^{-1} \\left[ {#2}; {#3}, {#4}, \\cdots, {#5} \\right]", 5]

macros["dctii"]  = ["\\mathcal{C}_{#1}      \\left[ {#2}; {#3}, {#4}, \\cdots, {#5} \\right]", 5]
macros["dctiii"] = ["\\mathcal{C}_{#1}^{-1} \\left[ {#2}; {#3}, {#4}, \\cdots, {#5} \\right]", 5]

macros["dstii"]  = ["\\mathcal{S}_{#1}      \\left[ {#2}; {#3}, {#4}, \\cdots, {#5} \\right]", 5]
macros["dstiii"] = ["\\mathcal{S}_{#1}^{-1} \\left[ {#2}; {#3}, {#4}, \\cdots, {#5} \\right]", 5]

macros["der"] = ["\\frac{d {#1}}{d {#2}}", 2]

mathjax3_config = {"TeX": {"Macros": macros}}
