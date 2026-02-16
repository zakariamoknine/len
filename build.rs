use std::path::PathBuf;

fn main() {
    let target_arch = std::env::var("CARGO_CFG_TARGET_ARCH")
        .expect("CARGO_CFG_TARGET_ARCH is undefined.");
    
    let linker_script = PathBuf::from(format!(
        "./kernel/arch/{target_arch}/{target_arch}.ld"
    ));

    if !linker_script.exists() {
        panic!("Unsupported architecture: {target_arch}");
    }

    println!("cargo:rerun-if-changed={}", linker_script.display());
    println!("cargo:rustc-link-arg=-T{}", linker_script.display());
}
