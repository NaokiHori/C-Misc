mathjax_path = "https://cdn.jsdelivr.net/npm/mathjax@2/MathJax.js?config=TeX-AMS-MML_HTMLorMML"

macros = dict()

macros["seq"] = ["#1 = 0, 1, \\cdots, #2", 2]

def for_rdft():
    macros["rdfttwiddle"] = ["\\exp \\left( #1 2 \\pi \\frac{#2}{#3} I \\right)", 3]
    macros["rdft"] = ["\\mathcal{F}_{#1} \\left[ #2_{#3}, #2_{#4}, \\cdots, #2_{#5} \\right]", 5]
    macros["irdft"] = ["\\mathcal{F}_{#1}^{-1} \\left[ #2_{#3}, #2_{#4}, \\cdots, #2_{#5} \\right]", 5]

def for_dct():
    macros["twiddle"] = ["{\\cos \\left[ \\frac{\\pi #1}{#2} \\right]}", 2]
    macros["dct"] = ["\\mathcal{C}_{#2}      \\left[ {#1}; {#3}, {#4}, \\cdots, {#5} \\right]", 5]
    macros["idct"] = ["\\mathcal{C}_{#2}^{-1} \\left[ {#1}; {#3}, {#4}, \\cdots, {#5} \\right]", 5]
    macros["defdct"] = (
            "\\hat{X}_k"
            "\\equiv"
            "2"
            "\\sum_{n = 0}^{N - 1}"
            "x_n"
            "\\twiddle{\\left( 2 n + 1 \\right) k}{2 N}"
            "\\equiv"
            "\\dct{N}{k}{x_0}{x_1}{x_{N - 1}}"
    )
    macros["defidct"] = (
            "x_n"
            "\\equiv"
            "2"
            "\\sum_{k = 0}^{N - 1}"
            "X_k"
            "\\twiddle{\\left( 2 n + 1 \\right) k}{2 N}"
            "\\equiv"
            "\\idct{N}{n}{X_0}{X_1}{X_{N - 1}}"
    )

for_rdft()
for_dct()

mathjax3_config = {"TeX": {"Macros": macros}}
