# YenLang

YenLang is a statically-typed programming language built on C++, designed specifically for malware development, security research, and related fields. With its simple syntax and powerful features, YenLang aims to provide researchers and developers with an accessible yet robust toolset for creating and testing proof-of-concepts, payloads, and other offensive security utilities. The language supports variable declarations, string concatenation, arithmetic operations, and basic conditional logic, making it ideal for both rapid prototyping and advanced malware development.

---

## Quick Start

### Code Example

```yen
var idade: int = 25;
var nome: string = "Joao";
print("Ola, " + nome);
print("Sua idade e: " + idade);

var nome2: string = "Joas";
var idade2: int = 18;
print("Seu nome e , " + nome2 + " " + idade2);
```

This code will produce the following output:

```
Ola, Joao
Sua idade e: 25
Seu nome e , Joas 18
```

## Installation

1. Clone this repository:
   ```bash
   git clone https://github.com/CyberSecurityUP/yenlang.git
   cd yenlang
   ```

2. Build the compiler:
   ```bash
   make
   ```

3. Run a YenLang script:
   ```bash
   Yen.exe code.yen
   ```

---

## Language Syntax

### Variable Declaration
Variables in YenLang are declared with the `var` keyword, followed by the variable name, type, and initial value.

```yen
var age: int = 30;
var name: string = "Alice";
```

### Printing
Use the `print` function to output text or variables to the console.

```yen
print("Hello, World!");
```

### String Concatenation
Strings can be concatenated using the `+` operator.

```yen
var greeting: string = "Hello, ";
var name: string = "Alice";
print(greeting + name);
```

### Conditional Statements
YenLang supports basic `if-else` logic.

```yen
var age: int = 18;
if (age >= 18) {
    print("You are an adult.");
} else {
    print("You are a minor.");
}
```

---

## Documentation

The full documentation is available in the `docs/` directory of this repository.

The generated documentation will include detailed descriptions of all supported syntax, keywords, and language features.

---

## License

YenLang is released under the [GNU General Public License v3.0](https://www.gnu.org/licenses/gpl-3.0.en.html). You are free to use, modify, and distribute this software, provided you adhere to the terms of the license.

---

## Contributing

We welcome contributions! Please follow the steps below:

1. Fork the repository.
2. Create a branch for your feature or bug fix.
3. Submit a pull request describing your changes.

---

## Acknowledgments

YenLang is developed with the goal of making programming accessible and fun. Special thanks to all contributors and users for their support!

---

## Contact

For questions or feedback, feel free to open an issue

Enjoy coding with YenLang!
