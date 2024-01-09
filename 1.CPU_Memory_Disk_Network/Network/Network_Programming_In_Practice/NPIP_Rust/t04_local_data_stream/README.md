### 说明

由于环境原因，相关的代码没有实现。比如下面的代码是在 linux 下运行的，但是在 windows 下是无法运行的，不方便开发。

```rust
#![feature(std_internals)]

use std::os::unix::net::{UnixListener, UnixStream};
use std::io::{Read, Write};
use std::fs;

fn main() {
    let local_path = "/tmp/t04_local_data_stream.sock";
    fs::remove_file(local_path).unwrap_or_default();

    let listener = UnixListener::bind(local_path).unwrap();
    println!("\nServer is listening on {} ...", local_path);

    for stream in listener.incoming() {
        let mut stream = stream.unwrap();
        let mut message = [0; 1024];
        let n = stream.read(&mut message).unwrap();
        let message = String::from_utf8_lossy(&message[..n]);
        println!("Received from client: {}", message);

        let send_line = format!("Hi, {}", message);
        stream.write_all(send_line.as_bytes()).unwrap();
    }
}

```
