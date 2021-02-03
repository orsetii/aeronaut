use std::env;

extern crate reqwest;

fn main() -> Result<(), Box<dyn std::error::Error>> {
    let args: Vec<String> = env::args().collect();

    //let mut res = reqwest::blocking::get(&args[1])?;

    Ok(())
}
