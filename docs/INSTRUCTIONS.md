# THORChain Ledger App
### Introduction

The THORChain Ledger app is used to safely sign transactions onto the [THORChain network](https://thorchain.org). All THORChain assets are supported including:
- **RUNE Native** ("RUNE" or "THOR.RUNE")
- **Synthetic Assets** ("BTC/BTC", "ETH/ETH" etc.)

**Note**: Other Layer 1 chain assets such as Bitcoin ("BTC.BTC") and Ethereum ("ETH.ETH") require their own dedicated Ledger apps.

The THORChain app runs on the Ledger [Nano S](https://shop.ledger.com/products/ledger-nano-s) and [Nano X](https://shop.ledger.com/pages/ledger-nano-x) devices.

### Technical capabilities
Transaction messages supported are:
- **MsgSend** -- sending assets (e.g. RUNE) to another address
- **MsgDeposit** -- For interacting with the network, for example to `BOND` a THORChain validator node.

The THORChain Ledger app uses HD path `m/44'/931'/0'/0/0`

### Requirements

- Ledger Nano S device with latest firmware `v2.0.0` (or higher), or
- Ledger Nano X device with latest firmware `v1.3.0` (or higher).
- Ledger device is initialised
- Computer with [Ledger Live](https://www.ledger.com/ledger-live) installed 
- Latest Chrome (for support with Websites) or [ASGARDEX desktop app](https://github.com/thorchain/asgardex-electron/releases) installed.

### Installation

- Open the **Settings** > **Experimental Features** in Ledger Live and enable **Developer Mode**
- Open the **Manager** tab
- Connect and unlock your Ledger device
- If asked, Allow Ledger manager on device
- Search for **THORChain** in the App catalog
- Click **Install** and wait for it to be loaded onto device

### Usage Instructions

* [ASGARDEX Desktop](https://github.com/thorchain/asgardex-electron/releases)  **implementation coming soon*
* [https://app.thorswap.finance/](https://app.thorswap.finance/)  **implementation coming soon* 

^ *Awaiting implementation by client apps*

### Support

THORChain is a decentralised network composed of anonymous validator nodes running the chain, aligned developers contributing code and many 3rd party services built on top that use the network. There does not exist a "company" or dedicated support center. However passionate community members are always eager to assist users. For community support, please see: 
- Telegram community (https://t.me/thorchain_org)
- Twitter (https://twitter.com/thorchain_org)
- THORSwap Discord (https://discord.gg/7drEFG6STF) 
- https://docs.thorchain.org 
