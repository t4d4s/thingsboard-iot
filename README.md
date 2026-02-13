# 📡👻 ThingsBoard IoT Library

> A C shared library and companion program for communicating with the [ThingsBoard IoT platform](https://thingsboard.io) over HTTP and MQTT.

---

## Overview

`ThingsBoard IoT Library` provides a shared library and CLI tool for sending telemetry data to ThingsBoard and monitoring remote procedure calls (RPC) from the platform. Both HTTP and MQTT transport APIs are supported.

### Status

| Feature | Status |
|---------|--------|
| HTTP communication with ThingsBoard | ✅ Done |
| MQTT communication with ThingsBoard | ✅ Done |
| RPC action monitoring over MQTT | ✅ Done |
| RPC action monitoring over HTTP | ✅ Done |
| Shared library | ✅ Done |
| Action control from platform to system | 🚧 Work in progress |

---

## Requirements

- GCC or Clang
- GNU Make
- libmosquitto (MQTT client library)
- libcurl (HTTP client library)
- A ThingsBoard instance with a device access token

On Ubuntu/Debian:

```bash
sudo apt install build-essential libmosquitto-dev libcurl4-openssl-dev
```

---

## Building

```bash
git clone https://github.com/t4d4s/thingsboard-iot.git
cd thingsboard-iot
make
```

---

## Usage

### MQTT mode

```bash
./tb <ACCESS_TOKEN> mqtt
```

### HTTP mode

```bash
./tb <ACCESS_TOKEN> http
```

### Interactive mode

```bash
./tb
```

Running without arguments will prompt you for an access token and topic. The default transport is **MQTT** — if you want HTTP, specify it explicitly as shown above.

---

## RPC Action Monitoring

Once the program is running, it will prompt you for an action code to monitor:

```
[RPC] Enter action code: test
```

After entering a code, the program will listen for matching RPC calls from the ThingsBoard dashboard. When an RPC request with that method name is received, the program responds accordingly.

To trigger an action, use the **RPC widget** in your ThingsBoard dashboard and call the matching method name.

---

## Contributions

Contributions are welcome! If you'd like to improve the project or add new features, please submit a pull request.

---

## Author

This project is maintained by [Tadas](https://github.com/t4d4s). Feel free to reach out with any questions or feedback.
