# Replay camera handoff verification

Both builds used the same offline AI-driven Monza Time Trial fixture on Steam depot 26566308. Only Swofty Core was enabled. The test explicitly dismissed the pause menu and waited up to 12 seconds after each replay exit. The saved-camera fallback was disabled.

| Build | Run | Last replay camera | Published after resuming |
| --- | --- | --- | --- |
| `7a8c5e98` | `shell-20261121-090650-1` | Cockpit | `kind=unknown flags=none valid=0 serial=24` |
| `7a8c5e98` | `shell-20261121-090650-1` | T-cam | `kind=cockpit flags=none valid=1 serial=16` |
| `05d5fc02` | `shell-20261121-090918-1` | Cockpit | `kind=cockpit flags=none valid=1 serial=25` |
| `05d5fc02` | `shell-20261121-090918-1` | T-cam | `kind=cockpit flags=none valid=1 serial=16` |

The fixed run logged `[view] recovered cockpit on return from Instant Replay` for the same-camera exit. The different-camera exit took the normal assignment path. Both game runs exited cleanly and the original Steam executable SHA-256 was restored.

Native tests covered a missing same-camera write, mismatched type, unknown type, a new live assignment and session reset. The PR head adds documentation to the tested native commit.
