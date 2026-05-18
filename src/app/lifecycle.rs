use crate::document::Document;
use crate::layout::Layout;
use super::types::App;

impl App {
    pub fn new(filenames: Vec<String>) -> Self {
        Self {
            doc: Document::new(),
            layout: Layout::new(),
            scroll_y: 0,
            filenames,
            current_file_index: 0,
            last_pattern: String::new(),
            last_search_dir: 1,
            search_failed: false,
            search_wrapped: false,
            show_help: false,
            show_line_numbers: false,
            running: true,
            terminal_cols: 0,
            terminal_rows: 0,
            error_message: None,
        }
    }

    pub fn init(&mut self) {
        if !self.filenames.is_empty() {
            let filename = self.filenames[self.current_file_index].clone();
            if let Err(e) = self.doc.load_file(filename) {
                self.error_message = Some(e.to_string());
            }
        } else {
            use std::io::IsTerminal;
            let stdin = std::io::stdin();
            if !stdin.is_terminal() {
                if let Err(e) = self.doc.load_stream(stdin) {
                    self.error_message = Some(e.to_string());
                }
            }
        }
    }

    pub fn switch_file(&mut self, index: usize) {
        if index < self.filenames.len() {
            self.current_file_index = index;
            self.doc.clear();
            let filename = self.filenames[self.current_file_index].clone();
            if let Err(e) = self.doc.load_file(filename) {
                self.error_message = Some(e.to_string());
            } else {
                self.error_message = None;
            }
            self.scroll_y = 0;
            self.layout.compute(&self.doc, self.terminal_cols);
        }
    }
}
