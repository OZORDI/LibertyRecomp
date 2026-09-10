#!/usr/bin/env python3
"""Run the real C++ adapter against isolated libserver with retryable write faults."""
from __future__ import annotations

import argparse
import hashlib
import http.client
import http.server
import json
import os
from pathlib import Path
import subprocess
import sys
import tempfile
import threading

sys.dont_write_bytecode = True
ROOT = Path(__file__).resolve().parents[4]
sys.path.insert(0, str(ROOT / "libserver/tests"))
from active_client_e2e_test import ServerProcess


def run(server_binary: Path, client_binary: Path, output: Path):
    output.mkdir(parents=True, exist_ok=True)
    writes = []
    mutex = threading.Lock()
    report = {}
    with tempfile.TemporaryDirectory(prefix="liberty-real-community-") as temporary:
        server = ServerProcess(str(server_binary), str(Path(temporary) / "state"), [])

        class Proxy(http.server.BaseHTTPRequestHandler):
            protocol_version = "HTTP/1.1"

            def log_message(self, *_):
                pass

            def process(self):
                payload = self.rfile.read(int(self.headers.get("Content-Length", "0")))
                record = None
                if self.command == "POST" and self.path == "/api/v2/stats/writes":
                    with mutex:
                        record = {"body_sha256": hashlib.sha256(payload).hexdigest(),
                                  "idempotency": self.headers.get("Idempotency-Key"),
                                  "request": json.loads(payload)}
                        writes.append(record)
                        inject = len(writes) <= 3
                    if inject:
                        record["status"] = 503
                        self.respond(503, json.dumps({"error": {
                            "code": "test_temporary_storage_fault", "message": "retryable test failure"}}).encode())
                        return
                connection = http.client.HTTPConnection("127.0.0.1", server.port, timeout=8)
                try:
                    headers = {key: value for key, value in self.headers.items()
                               if key.lower() not in {"host", "connection", "content-length"}}
                    connection.request(self.command, self.path, payload or None, headers)
                    response = connection.getresponse()
                    data = response.read()
                    if record is not None:
                        record["status"] = response.status
                    self.respond(response.status, data)
                finally:
                    connection.close()

            def respond(self, status, data):
                self.send_response(status)
                self.send_header("Content-Length", str(len(data)))
                self.send_header("Content-Type", "application/json")
                self.send_header("Connection", "close")
                self.end_headers()
                try:
                    self.wfile.write(data)
                except (BrokenPipeError, ConnectionResetError):
                    pass
                self.close_connection = True

            do_GET = process
            do_POST = process
            do_PUT = process
            do_PATCH = process
            do_DELETE = process

        proxy = http.server.ThreadingHTTPServer(("127.0.0.1", 0), Proxy)
        proxy.daemon_threads = True
        worker = threading.Thread(target=proxy.serve_forever, daemon=True)
        worker.start()
        try:
            result = subprocess.run([str(client_binary),
                                     "http://127.0.0.1:" + str(proxy.server_port),
                                     "--write-failure"], capture_output=True, text=True,
                                    timeout=60, env=dict(os.environ, PYTHONDONTWRITEBYTECODE="1"))
            (output / "real-client-runtime.log").write_text(result.stdout + result.stderr)
            same_intent = bool(writes) and len({(w["body_sha256"], w["idempotency"])
                                               for w in writes}) == 1
            correct_write_recovery = [w.get("status") for w in writes] == [503, 503, 503, 204]
            report = {"client_exit": result.returncode, "write_attempts": writes,
                      "original_sequence_and_idempotency_preserved": same_intent,
                      "flush_retried_after_write_failure": correct_write_recovery,
                      "host_worker_qos_verified": "client-host-qos=passed" in result.stdout,
                      "absent_host_worker_unreachable": "client-no-host-qos=passed" in result.stdout,
                      "actual_client_relay_verified": "client-relay=passed" in result.stdout}
            report["passed"] = result.returncode == 0 and same_intent and correct_write_recovery
        finally:
            proxy.shutdown()
            proxy.server_close()
            worker.join(timeout=2)
            server.stop()
            (output / "real-client-server.log").write_text(server.diagnostics())
            server.close()
            (output / "real-client-result.json").write_text(json.dumps(report, indent=2) + "\n")
    print(json.dumps(report, indent=2))
    return 0 if report.get("passed") else 1


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("server", type=Path)
    parser.add_argument("client", type=Path)
    parser.add_argument("--output", required=True, type=Path)
    args = parser.parse_args()
    raise SystemExit(run(args.server.resolve(), args.client.resolve(), args.output.resolve()))
