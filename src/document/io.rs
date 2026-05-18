use std::fs::File;
use std::io::{self, BufRead, BufReader};
use std::path::Path;
use super::types::Document;
use crate::errors::InklessError;

impl Document {
    pub fn load_file<P: AsRef<Path>>(&mut self, path: P) -> Result<(), InklessError> {
        let file = File::open(&path)?;
        self.load_stream(file)
    }

    pub fn load_stream<R: io::Read>(&mut self, reader: R) -> Result<(), InklessError> {
        let buf_reader = BufReader::new(reader);
        for line in buf_reader.lines() {
            let line = line?;
            self.raw_lines.push(line);
        }
        Ok(())
    }
}
