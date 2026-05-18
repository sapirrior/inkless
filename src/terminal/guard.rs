use crossterm::{
    cursor,
    terminal::{self, EnterAlternateScreen, LeaveAlternateScreen},
    ExecutableCommand,
};
use std::io::{self, Write};
use crate::errors::InklessError;

pub struct TerminalGuard;

impl TerminalGuard {
    pub fn new() -> Result<Self, InklessError> {
        terminal::enable_raw_mode()
            .map_err(|e| InklessError::Terminal(format!("Failed to enable raw mode: {}", e)))?;
        
        io::stdout()
            .execute(EnterAlternateScreen)
            .map_err(|e| InklessError::Terminal(format!("Failed to enter alternate screen: {}", e)))?
            .execute(cursor::Hide)
            .map_err(|e| InklessError::Terminal(format!("Failed to hide cursor: {}", e)))?;
            
        Ok(Self)
    }
}

impl Drop for TerminalGuard {
    fn drop(&mut self) {
        let mut stdout = io::stdout();
        let _ = stdout.execute(cursor::Show);
        let _ = stdout.execute(LeaveAlternateScreen);
        let _ = terminal::disable_raw_mode();
        let _ = stdout.flush();
    }
}
