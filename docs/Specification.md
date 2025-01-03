# Interface Specification

## Actions

- [x] Create a new neural network
- [x] Set configuration values from within the interface
- [x] Train nn using `.dat` file
- [x] Feed nn test values and get results
- [x] Save and Load neural networks from `.sav` files
- [x] Help
- [x] Quit

## Commands

- `nn [LAYER-NEURONS]...`
- `load [TYPE] [PATH]`
- `set [KEY] [VALUE]`
- `train`
- `feed [INPUT-VALUES]...`
- `save [PATH]`
- `help`
- `quit`
- `print [VALUE]`


## Examples

```
> nn 3 2 1
info Neural network 3 2 1 created
> load data ./data/xor.dat
info Loaded ./data/xor.dat
warn xor.dat requires a network with 2 inputs. NN has been modified to match data requirements.
> set epochs 50000
> set trainingrate 1.7
> set activation RELU
> train 
info Minimum loss reached 0.0172
> feed 0 1
info 0.982
> save ./nn.sav
> load nn ./nn.sav
info Loaded nn from ./nn.sav
> feed 0 1
info 0.982
> help
<help-message>
> quit
```
