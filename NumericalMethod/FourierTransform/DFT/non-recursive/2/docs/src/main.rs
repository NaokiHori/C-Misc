use komainu::{Document, Expr};

fn main() -> Result<(), Box<dyn std::error::Error>> {
    let template = std::fs::read_to_string("template/README.komainu.md")?;
    let mut doc = Document::new(template);
    // constant
    doc.symbol("pi", r"\pi");
    doc.symbol("imaginary_unit", "i");
    // common symbol
    doc.symbol("dof", "N");
    doc.symbol("radix", "r");
    doc.symbol("quotient", "q");
    doc.symbol("index0", "m");
    doc.symbol("index1", "k");
    doc.symbol("index2", "n");
    doc.symbol("index3", "l");
    doc.symbol("index4", "p");
    // common operator
    doc.operator("fraction", 2, |args| Expr::fraction(&args[0], &args[1]));
    doc.operator("sum_between", 4, {
        |args| Expr::sum_between(&args[0], &args[1], &args[2], &args[3])
    });
    // specific symbol and operator
    let physical_symbol = doc.symbol("physical_symbol", "f");
    doc.operator("physical", 1, {
        let physical_symbol = physical_symbol.clone();
        move |args| Expr::subscript(&physical_symbol, &args[0])
    });
    let spectral_symbol = doc.symbol("spectral_symbol", "F");
    doc.operator("spectral", 1, {
        let spectral_symbol = spectral_symbol.clone();
        move |args| Expr::subscript(&spectral_symbol, &args[0])
    });
    let twiddle_symbol = doc.symbol("twiddle_symbol", "W");
    doc.operator("twiddle", 3, {
        let twiddle_symbol = twiddle_symbol.clone();
        move |args| {
            Expr::superscript(
                Expr::subscript(&twiddle_symbol, &args[0]),
                &args[1] * &args[2],
            )
        }
    });
    let exponential_symbol = doc.symbol("exponential_symbol", r"\exp");
    doc.operator("exponential", 1, {
        let exponential_symbol = exponential_symbol.clone();
        move |args| Expr::function(&exponential_symbol, args)
    });
    let markdown = doc.render()?;
    std::fs::write("output/README.md", markdown)?;
    Ok(())
}
