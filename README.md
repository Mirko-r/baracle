# baracle
Let's make a text editor like in the 70's
 
![baracle](./img/baracle.png)

# Installation

## Arch Linux and derivatives (AUR)

### Stable
package: [baracle](https://aur.archlinux.org/packages/baracle)<br>

Use an [AUR helper](https://wiki.archlinux.org/title/AUR_helpers) or

```bash
git clone https://aur.archlinux.org/baracle.git
makepkg -si
```
### Unstable
package: [baracle-git](https://aur.archlinux.org/packages/baracle-git)<br>

Use an [AUR helper](https://wiki.archlinux.org/title/AUR_helpers) or

```bash
git clone https://aur.archlinux.org/baracle-git.git
makepkg -si
```

## Manual

### Stable

- go to [the releases page](https://github.com/Mirko-r/baracle/releases)
- download the `baracle.c` of the version you want
- open your terminal in the folder where you downloaded it and run:

```bash
gcc baracle.c -o baracle -Wall -Wextra -pedantic -std=c99
./baracle
```

### Unstable

```bash
git clone https://github.com/Mirko-r/baracle
cd baracle/build
make 
./baracle
```

## Debian\Ubuntu and derivatives

Soon™ (seriously though, soon)

## Sample Usage

| Command              | Function                                                               |
| -------------------- | ---------------------------------------------------------------------- |
| `ctrl+s`| save file       |
| `ctrl+q`| quit baracle    |
| `ctrl+f`| find a string	|

## Contributing

1. [Fork it](<https://github.com/Mirko-r/baracle/fork>)
2. Create your feature branch (`git checkout -b feature/fooBar`)
3. Commit your changes (`git commit -am 'Add some fooBar'`)
4. Push to the branch (`git push origin feature/fooBar`)
5. Create a new Pull Request

## 💲Support my work

[![paypal](https://img.shields.io/badge/PayPal-00457C?style=for-the-badge&logo=paypal&logoColor=white)](https://paypal.me/stupidamentepod)
