### Description
Finds differences between seeds and seeds from offsets.

- First prompt accepts hex numbers
- Second prompt accepts hex numbers or signed offsets (e.g. `+200`, `-200`)
- Both prompts support whitespace
- Both prompts accept `'x'` to quit

### Example Usage:

```
Please input first seed: 0xe529a300
Please input second seed or offset: BB AF 01 66

0xe529a300 => 0xbbaf0166
-1650
```

```
Please input first seed: 2F167E28
Please input second seed or offset: + 50

0x2f16e28 + 50 => 0x254f7512
```

