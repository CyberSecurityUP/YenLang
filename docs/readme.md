Here is a concise **Documentation (Docs)** for **YenLang**:

---

# YenLang Documentation

## Overview

YenLang is a C++-based statically-typed programming language tailored for malware development and offensive security research. Its goal is to provide a clean and efficient syntax for prototyping malicious code, exploring proof-of-concept payloads, and creating utilities for security testing.

---

## Language Basics

### Variable Declaration

Declare variables using the `var` keyword, followed by the variable name, type, and value. Supported types include:

- `int` - Integer numbers.
- `string` - Text data.

**Example:**

```yen
var age: int = 30;
var name: string = "MalwareDev";
```

---

### Output to Console

Use the `print` function to display text or variables.

**Example:**

```yen
var message: string = "YenLang rocks!";
print(message);
print("This is YenLang for offensive security.");
```

---

### String Concatenation

Concatenate strings using the `+` operator.

**Example:**

```yen
var part1: string = "Hello, ";
var part2: string = "World!";
print(part1 + part2);
```

---

### Conditional Statements

Implement conditional logic with `if` and `else`.

**Example:**

```yen
var isMalicious: int = 1;
if (isMalicious) {
    print("This payload is active.");
} else {
    print("No malicious behavior detected.");
}
```

---

### Arithmetic Operations

Perform arithmetic operations with `+`, `-`, `*`, and `/`.

**Example:**

```yen
var a: int = 10;
var b: int = 20;
print(a + b); // Output: 30
```

---

## Advanced Topics

### Interaction with Input

Use `input` to read user-provided data.

**Example:**

```yen
print("Enter a command: ");
var command: string = input();
print("You entered: " + command);
```

---

### Creating Variables Dynamically

Define and initialize variables at runtime.

**Example:**

```yen
var key: string = "encryptionKey123";
var payload: string = "Execute payload with " + key;
print(payload);
```

---

## Running YenLang Code

1. Write your script in a `.yen` file.
2. Compile and execute using the provided compiler:

   ```bash
   ./yenlang <script.yen>
   ```

---

## Limitations and Focus Areas

YenLang is designed with offensive security in mind. It is **not** a general-purpose language but is tailored for:

- Malware prototyping.
- Proof-of-concept development.
- Payload generation.

**Note:** Always use YenLang responsibly and within legal and ethical guidelines.

---

For more details, explore the full documentation or reach out to the maintainer.

Enjoy experimenting with **YenLang**!