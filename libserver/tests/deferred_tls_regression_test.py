#!/usr/bin/env python3
"""Exercise detached TLS poll connections, event delivery, and bounded shutdown."""
from __future__ import annotations

import argparse
import concurrent.futures
import http.client
import json
from pathlib import Path
import sys
import tempfile
import time

sys.dont_write_bytecode = True
from active_client_e2e_test import Identity, enroll
from social_lobby_e2e_test import session_body
from production_hosting_test import ServerProcess, generate_certificates, trusted_context


class SecureApi:
    def __init__(self, port, context):
        self.port, self.context = port, context

    def request(self, method, path, token=None, body=None, expected=200, error_code=None, extra_headers=None):
        headers = {"Content-Type": "application/json"}
        if extra_headers:
            headers.update(extra_headers)
        if token:
            headers["Authorization"] = "Bearer " + token
        connection = http.client.HTTPSConnection("127.0.0.1", self.port,
                                                  context=self.context, timeout=5)
        try:
            connection.request(method, path, None if body is None else json.dumps(body), headers)
            response = connection.getresponse()
            payload = response.read()
            result = json.loads(payload) if payload else None
            assert response.status == expected, (path, response.status, result)
            if error_code:
                assert result["error"]["code"] == error_code, result
            return result
        finally:
            connection.close()


def run(binary, output=None):
    report = {}
    with tempfile.TemporaryDirectory(prefix="libserver-deferred-tls-") as folder:
        folder = Path(folder)
        ca, chain, key = generate_certificates(folder)
        context = trusted_context(ca)
        server = ServerProcess(str(binary), ["--data-dir", str(folder / "state"),
                                "--tls-cert", str(chain), "--tls-key", str(key)], context=context)
        try:
            server.require_ready()
            api = SecureApi(server.port, context)
            host, peer = [Identity(0xAE7100 + i, "tls-deferred-" + str(i)) for i in range(2)]
            for identity in [host, peer]:
                enroll(api, identity)
            room = api.request("POST", "/api/v2/sessions", host.access_token,
                                session_body("0x0000000000ae7200", host, [host, peer]), expected=201)
            with concurrent.futures.ThreadPoolExecutor(max_workers=1) as pool:
                waiting = pool.submit(api.request, "GET", "/api/v2/events?wait_ms=30000",
                                      peer.access_token)
                time.sleep(0.1)
                assert not waiting.done()
                began = time.monotonic()
                api.request("GET", "/health/live")
                health_seconds = time.monotonic() - began
                api.request("POST", "/api/v2/chat/messages", host.access_token,
                             {"session_id": room["session_id"], "channel": "all",
                              "target_xuids": [], "sequence": 1, "text": "TLS delivery"}, expected=201,
                             extra_headers={"Idempotency-Key": "tls-deferred-message"})
                events = waiting.result(timeout=4)["events"]
                assert any(e["payload"]["text"] == "TLS delivery" for e in events)
                report["tls_event_delivery"] = True
                report["health_seconds_during_tls_poll"] = health_seconds
            # An idle TLS subscription must not make graceful process shutdown
            # wait for its 30-second application deadline.
            after = str(events[-1]["id"])
            with concurrent.futures.ThreadPoolExecutor(max_workers=1) as pool:
                waiting = pool.submit(api.request, "GET", "/api/v2/events?wait_ms=30000&after=" + after,
                                      peer.access_token)
                time.sleep(0.1)
                assert not waiting.done()
                began = time.monotonic()
                server.stop()
                elapsed = time.monotonic() - began
                assert elapsed < 2, elapsed
                report["shutdown_seconds_with_pending_tls_poll"] = elapsed
                try:
                    waiting.result(timeout=3)
                except (OSError, http.client.HTTPException):
                    pass
            report["passed"] = True
        finally:
            server.stop()
            if output:
                output.mkdir(parents=True, exist_ok=True)
                (output / "deferred-tls-process.log").write_text(server.diagnostics())
                (output / "deferred-tls-result.json").write_text(json.dumps(report, indent=2) + "\n")
            server.close()
    print(json.dumps(report, indent=2))
    return 0


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("server", type=Path)
    parser.add_argument("--output", type=Path)
    args = parser.parse_args()
    raise SystemExit(run(args.server.resolve(), args.output))
