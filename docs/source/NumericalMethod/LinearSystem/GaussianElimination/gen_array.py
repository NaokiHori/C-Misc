def a(row, col):
    return f"a_{{{row},{col}}}"

def b(row):
    return f"b_{{{row}}}"

def cd():
    return "\\cdots"

def vd():
    return "\\vdots"

def dd():
    return "\\ddots"

def gen(elements):
    result = ""
    result += ".. math::\n"
    result += "\n"
    result += "    \\begin{bmatrix}\n"
    for element in elements:
        s = "        "
        s += " & ".join([item.rjust(11) for item in element])
        s += " \\\\\n"
        result += s
    result += "    \\end{bmatrix}"
    return result

def text1(file_name):
    elements = list()
    elements.append([vd(), vd(), dd(), vd(), vd(), vd(),   "", vd(), vd(), vd()])
    elements.append(["0", "0", cd(), "1", a("i-1", "i  "), a("i-1", "i+1"), cd(), a("i-1", "n-2"), a("i-1", "n-1"), b("i-1")])
    elements.append(["0", "0", cd(), "0", a("i  ", "i  "), a("i  ", "i+1"), cd(), a("i  ", "n-2"), a("i  ", "n-1"), b("i  ")])
    elements.append(["0", "0", cd(), "0", a("i+1", "i  "), a("i+1", "i+1"), cd(), a("i+1", "n-2"), a("i+1", "n-1"), b("i+1")])
    elements.append([vd(), vd(),   "", vd(), vd(), vd(), dd(), vd(), vd(), vd()])
    with open(file_name, "w") as f:
        f.write(gen(elements) + ".\n")

def text2(file_name):
    elements = list()
    elements.append([vd(), vd(), dd(), vd(), vd(), vd(),   "", vd(), vd(), vd()])
    elements.append(["0", "0", cd(), "1", a("i-1", "i  "), a("i-1", "i+1"), cd(), a("i-1", "n-2"), a("i-1", "n-1"), b("i-1")])
    elements.append(
            [
                "0",
                "0",
                cd(),
                "0",
                "1",
                a("i  ", "i+1") + " := " + a("i  ", "i+1") + " / " + a("i  ", "i  "),
                cd(),
                a("i  ", "n-2") + " := " + a("i  ", "n-2") + " / " + a("i  ", "i  "),
                a("i  ", "n-1") + " := " + a("i  ", "n-1") + " / " + a("i  ", "i  "),
                b("i  ") + " := " + b("i  ") + " / " + a("i  ", "i  ")
            ]
    )
    elements.append(["0", "0", cd(), "0", a("i+1", "i  "), a("i+1", "i+1"), cd(), a("i+1", "n-2"), a("i+1", "n-1"), b("i+1")])
    elements.append([vd(), vd(),   "", vd(), vd(), vd(), dd(), vd(), vd(), vd()])
    with open(file_name, "w") as f:
        f.write(gen(elements) + ".\n")

def text3(file_name):
    elements = list()
    elements.append([vd(), vd(), dd(), vd(), vd(), vd(),   "", vd(), vd(), vd()])
    elements.append(["0", "0", cd(), "1", a("i-1", "i  "), a("i-1", "i+1"), cd(), a("i-1", "n-2"), a("i-1", "n-1"), b("i-1")])
    elements.append(["0", "0", cd(), "0", "1",             a("i  ", "i+1"), cd(), a("i  ", "n-2"), a("i  ", "n-1"), b("i  ")])
    elements.append(
            [
                "0",
                "0",
                cd(),
                "0",
                "0",
                a("i+1", "i+1") + " := " + a("i+1", "i+1") + " - " + a("i+1", "i  ") + a("i  ", "i+1"),
                cd(),
                a("i+1", "n-2") + " := " + a("i+1", "n-2") + " - " + a("i+1", "i  ") + a("i  ", "n-2"),
                a("i+1", "n-1") + " := " + a("i+1", "n-1") + " - " + a("i+1", "i  ") + a("i  ", "n-1"),
                b("i+1")        + " := " + b("i+1")        + " - " + a("i+1", "i  ") + b("i  ")       ,
            ]
    )
    elements.append([vd(), vd(),   "", vd(), vd(), vd(), dd(), vd(), vd(), vd()])
    with open(file_name, "w") as f:
        f.write(gen(elements) + ".\n")

text1("math/1.txt")
text2("math/2.txt")
text3("math/3.txt")

