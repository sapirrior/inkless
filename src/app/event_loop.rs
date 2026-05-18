use super::types::App;

impl App {
    pub fn run(&mut self) {
        self.init();
        
        let _guard = match crate::terminal::TerminalGuard::new() {
            Ok(g) => g,
            Err(e) => {
                eprintln!("Initialization error: {}", e);
                return;
            }
        };

        let (cols, rows) = match crate::terminal::get_size() {
            Ok(s) => s,
            Err(e) => {
                self.error_message = Some(e.to_string());
                (80, 24) // Fallback size
            }
        };
        self.terminal_cols = cols;
        self.terminal_rows = rows;
        
        self.layout.compute(&self.doc, cols);

        while self.running {
            crate::view::render_screen(self);
            let key = crate::input::read_key();
            match key {
                crate::input::Key::Unknown => continue,
                _ => crate::commands::dispatch(self, key),
            }
        }
    }
}
