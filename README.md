# Squid

Squid is a program that monitors the output of the [albertobsd/keyhunt](https://github.com/albertobsd/keyhunt) program and sends private key information of Bitcoin addresses to a user-defined server.<br>
And before I forget, this is my first program in C ^^

## Requirements

- **GCC** (GNU Compiler Collection)
- **libcurl** (library for data transfers with URL)

## Installation

### 1. Install GCC and libcurl

To install GCC and the libcurl library, follow the instructions below based on your operating system.

#### On Debian/Ubuntu-based systems

```bash
sudo apt update
sudo apt install build-essential libcurl4-openssl-dev
```

#### On Red Hat/Fedora-based systems
```bash
sudo dnf install gcc libcurl-devel
```

#### On macOS
You can install GCC and libcurl using Homebrew:
```bash
brew install gcc curl
```

### 2. Clone the repository
```bash
git clone https://github.com/your_username/squid.git
cd squid
```

### 3. Compile the program
Run the following command to compile the program:
```bash
gcc -o squid squid.c -lcurl
```

### 4. Set Environment Variables
To set the environment variables defined in your .env file, run the following command:
```bash
export $(cat .env | xargs)
```

### Execution

To run the program, use the command:
```bash
./squid
```

### Output
The program will display information about the found private key and confirm the sending of that information to the server. An example output is:

```
_bit_range: 50

[+] Waiting for the bit range key '2^49...2^50-1'

[+] Found file: 'KEYFOUNDKEYFOUND.txt'

[+] Found Key: 22bd43c2e9354
[+] Key HEX: 00000000000000000000000000000000000000000000000000022bd43c2e9354
{"status_code":200,"message":"Request processed successfully","data":{"success":"Ok"}}
[+] Key successfully sent to the server!
```

### Contribution
Feel free to contribute improvements or fixes. Just fork the repository and submit a pull request.
