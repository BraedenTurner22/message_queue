# mq — a distributed message queue in C++20

A Kafka-style replicated log built from scratch. Producers and consumers talk to
brokers over gRPC. Each topic is split into partitions, and each partition is an
append-only log replicated to a leader and in-sync followers. A controller
decides who leads and handles failover. The goal is a small system with
precisely stated guarantees (for example: no acked write is lost with
`acks=all` and `min.insync.replicas=2`), backed by benchmarks and randomized
fault-injection tests.

## Roadmap

| Phase | What | Status |
|---|---|---|
| 0 | Repo, CMake presets, vcpkg, sanitizers, CI | ✅ |
| 1 | Single-node storage engine: record batches, segments, offset index, crash recovery, retention | ⏳ next |
| 2 | gRPC broker API (Produce / Fetch with long-poll) and CLI tools | |
| 3 | Topics and partitions, client library with batching, benchmarks | |
| 4 | Replication: ISR, high watermark, acks, leader epochs | |
| 5 | Controller, automatic failover, fault-injection harness, history checker | |
| 6 | Consumer groups and committed offsets | |
| 7 | Stretch goals: Raft metadata quorum, idempotent producers, compaction, io_uring, metrics | |

## Layout

```
proto/          .proto service definitions (Phase 2)
src/common/     shared utilities (CRC32C, errors, byte helpers)
src/storage/    log, segments, index (Phase 1)
src/broker/     gRPC server (Phase 2)
src/client/     producer/consumer client library (Phase 3)
tests/          GoogleTest unit and integration tests
bench/          Google Benchmark microbenchmarks and load generator
tools/          CLI tools and scripts
docker/         multi-broker Docker Compose clusters (Phase 4)
```

## Building

Prerequisites: CMake ≥ 3.25, Ninja, a C++20 compiler (Clang 16+ or GCC 13+), and
[vcpkg](https://github.com/microsoft/vcpkg) with `VCPKG_ROOT` set.

```sh
# one-time setup (macOS)
brew install ninja
git clone https://github.com/microsoft/vcpkg.git ~/vcpkg
~/vcpkg/bootstrap-vcpkg.sh -disableMetrics
echo 'export VCPKG_ROOT="$HOME/vcpkg"' >> ~/.zshrc   # then open a new shell
```

```sh
cmake --preset debug && cmake --build --preset debug && ctest --preset debug
```

| Preset | Use |
|---|---|
| `debug` | Day-to-day development |
| `release` | Benchmarks (only trust numbers from this one) |
| `asan` | AddressSanitizer + UBSan |
| `tsan` | ThreadSanitizer |
| `tidy` | Build library code with clang-tidy as errors |

Dependencies come from `vcpkg.json` (manifest mode) and are built
automatically on the first configure.
