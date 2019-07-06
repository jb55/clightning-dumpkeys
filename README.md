
# clightning-dumpkeys

Dump funding wallet xpubs and xprv from your clightning node.

**WARNING** just saving your xprvs is not enough to backup your
clightning node. There are other channel state (p2wsh) outputs that are
not dumped here. Please use the recommended backup method provided by
clightning (as of this writing there aren't any, but that should
change soon).


## usage

    $ ./clightning-dumpkeys ~/.lightning/hsm_secret

It will dump your root+extended xprv/xpub with [output descriptors](https://github.com/bitcoin/bitcoin/blob/master/doc/descriptors.md):

```
xprv9s21ZrQH143K2W9UqiTJL3qndXzRkEuzAoNwd6VLRu2VDtX6tACVz9q8BzDmJDkB6c6QQnfEWWvLtxB9M68XVpmcJCQodFrr843paWAXGeD	root private
xpub661MyMwAqRbcEzDwwjzJhBnXBZpv9hdqY2JYRUtwzEZU6grFRhWkXx9c3HJ4EKR1Nvdwf5U3VoekstoKSKjfcJYhRrhMYeEzZzu2h7uZAQX	root public

xprv9wPk6zRy29x8BLXUHWh8ssqf89xQWtrZeLUgEXyUKSqNdUJhXTJsMtdLCexXxhgxPYUFpQvUhz2WAyhh3uCXSLedbHkvX7jTnX4xQofjdbA	extended private
combo(xprv9wPk6zRy29x8BLXUHWh8ssqf89xQWtrZeLUgEXyUKSqNdUJhXTJsMtdLCexXxhgxPYUFpQvUhz2WAyhh3uCXSLedbHkvX7jTnX4xQofjdbA/*)#u4tc9nwu	extended private descriptor
xpub6AP6WVxrrXWRPpbwPYE9F1nPgBntvMaR1ZQH2vP5snNMWGdr4zd7ugwp3wukcTUxKu2rLCN9VBQAW3xioATnEWjZvQpx9cybj1jztJHJyp7	extended public
combo(xpub6AP6WVxrrXWRPpbwPYE9F1nPgBntvMaR1ZQH2vP5snNMWGdr4zd7ugwp3wukcTUxKu2rLCN9VBQAW3xioATnEWjZvQpx9cybj1jztJHJyp7/*)#f64dm6yh	extended public descriptor
```

## building

You need to have the `secp256k1` library installed for this to compile

    $ make


## output descriptors

You can load your clightning wallet keys into your full node with importmulti.

First, save the `extended public (or private) descriptor` into a
`descriptors.json` file, this will be used for importing into bitcoin-core:

`descriptors.json`:
```json
[
  {
    "desc": "combo(xpub6AP6WVxrrXWRPpbwPYE9F1nPgBntvMaR1ZQH2vP5snNMWGdr4zd7ugwp3wukcTUxKu2rLCN9VBQAW3xioATnEWjZvQpx9cybj1jztJHJyp7/*)#f64dm6yh",
    "keypool": false,
    "timestamp": "now",
    "watchonly": true,
    "internal": false,
    "range": [0, 1000]
  }
]
```

Change `watchonly` to `false` and `keypool` to `true` if you're using the
extended private descriptor.

`watchonly` means we're only going to watch these addresses, not spend them.

`keypool` adds these keys into bitcoin-cores internal pool of keys, these are
used when creating new addresses from the api, etc.

Now we can import this into bitcoin:

    $ bitcoin-cli createwallet clightning true
    $ bitcoin-cli -rpcwallet=clightning importmulti "$(cat descriptors.json)" '{"rescan": false}'
    $ bitcoin-cli -rpcwallet=clightning rescanblockchain 503500
