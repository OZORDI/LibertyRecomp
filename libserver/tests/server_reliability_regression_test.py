#!/usr/bin/env python3
"""Behavioral regressions for the reviewed community-server failures.

Every process binds loopback and owns temporary state. No installed account data
or game instance is used. Request constants and test arithmetic are Python.
"""
from __future__ import annotations

import argparse
import concurrent.futures
import copy
import hashlib
import http.client
import json
import os
from pathlib import Path
import shutil
import subprocess
import sys
import tempfile
import threading
import time
import traceback

sys.dont_write_bytecode = True
from active_client_e2e_test import Identity, ServerProcess, base64url, enroll, wire_hex, reserve_port
from social_lobby_e2e_test import session_body


class Api:
    def __init__(self, port: int):
        self.port = port

    def raw(self, method, path, token=None, body=None, timeout=8, extra_headers=None):
        headers = {"Content-Type": "application/json"}
        if token:
            headers["Authorization"] = "Bearer " + token
        if extra_headers:
            headers.update(extra_headers)
        connection = http.client.HTTPConnection("127.0.0.1", self.port, timeout=timeout)
        try:
            connection.request(method, path, None if body is None else json.dumps(body), headers)
            response = connection.getresponse()
            data = response.read()
            return response.status, json.loads(data) if data else None
        finally:
            connection.close()

    def request(self, method, path, token=None, body=None, expected=200,
                error_code=None, extra_headers=None):
        status, result = self.raw(method, path, token, body, extra_headers=extra_headers)
        assert status == expected, (method, path, status, result, "expected", expected)
        if error_code:
            assert result["error"]["code"] == error_code, result
        return result


class Suite:
    def __init__(self, api, identities):
        self.api = api
        self.host, self.peer, self.other = identities
        self.ordinal = 0xAE0000
        self.results = []

    def run(self, name, fn):
        started = time.monotonic()
        try:
            evidence = fn()
            self.results.append({"test": name, "passed": True,
                                 "seconds": time.monotonic() - started,
                                 "evidence": evidence})
        except Exception as error:
            traceback.print_exc()
            self.results.append({"test": name, "passed": False,
                                 "seconds": time.monotonic() - started,
                                 "error": repr(error)})
        print(json.dumps(self.results[-1]), flush=True)

    def create(self, members=None, ranked=False):
        self.ordinal += 1
        members = members or [self.host]
        body = session_body(wire_hex(self.ordinal, 16), members[0], members,
                            lifecycle_state=0, state="lobby")
        body.update(flags=0x3E if ranked else 0x2E, ranked=ranked,
                    exchange_key="0x" + hashlib.sha256(str(self.ordinal).encode()).hexdigest()[:32],
                    nonce=wire_hex(self.ordinal + 100, 16),
                    host_machine_id=members[0].machine_id)
        for row, identity in zip(body["members"], members):
            row.update(machine_id=identity.machine_id, online_port=37000)
        return self.api.request("POST", "/api/v2/sessions", members[0].access_token,
                                body, expected=201)

    def current(self, session, who=None):
        return self.api.request("GET", "/api/v2/sessions/" + session["session_id"],
                                (who or self.host).access_token)

    def modify(self, session, who=None, expected=200, **changes):
        who = who or self.host
        current = self.current(session, who)
        return self.api.request("PATCH", "/api/v2/sessions/" + current["session_id"],
                                who.access_token,
                                {"expected_revision": current["revision"], **changes},
                                expected=expected)

    def route(self, session, who):
        return self.api.request("POST", "/api/v2/relay/routes", who.access_token,
                                {"session_id": session["session_id"], "local_port": 37000})

    def stat(self, session, columns, view=0x6D, expected=204, target=None):
        sequence = self.api.request("POST", "/api/v2/stats/sequences", self.host.access_token,
                                    {"session_id": session["session_id"]}, expected=201)["sequence"]
        body = {"session_id": session["session_id"], "sequence": sequence,
                "views": [{"view_id": wire_hex(view, 8), "rows": [{
                    "xuid": target or self.host.xuid, "columns": columns}]}]}
        response = self.api.request("POST", "/api/v2/stats/writes", self.host.access_token,
                                    body, expected=expected)
        return body, response

    def register(self, session, members):
        for who in members:
            current = self.current(session, who)
            status, result = self.api.raw("POST", "/api/v2/sessions/" + session["session_id"] +
                                         "/arbitration", who.access_token,
                                         {"expected_revision": current["revision"],
                                          "registration_duration_seconds": 300,
                                          "flags": 0, "nonce": current["nonce"]})
            assert status in (200, 202), (status, result)
        current = self.current(session)
        status, result = self.api.raw("POST", "/api/v2/sessions/" + session["session_id"] +
                                     "/arbitration", self.host.access_token,
                                     {"expected_revision": current["revision"],
                                      "registration_duration_seconds": 300,
                                      "flags": 0, "nonce": current["nonce"]})
        assert status == 200, (status, result)
        return result

    def identities(self):
        path = "/api/v3/storage/0x545407F2/3/Prog_ACH"
        original = base64url(bytes([7]) * 604)
        self.api.request("PUT", path, self.host.access_token, {"blob": original})
        impostor = Identity(0xAEF00D, "review-case-impostor")
        impostor.xuid = "0x" + self.host.xuid[2:].upper()
        assert int(impostor.xuid, 16) == int(self.host.xuid, 16)
        self.api.request("POST", "/api/v2/devices/challenge", body={
            "device_id": impostor.device_id, "xuid": impostor.xuid,
            "public_key": impostor.public_key}, expected=409,
            error_code="xuid_binding_conflict")
        self.api.request("GET", path, self.peer.access_token, expected=404)
        assert self.api.request("GET", path, self.host.access_token)["blob"] == original
        zero = Identity(0, "review-zero")
        self.api.request("POST", "/api/v2/devices/challenge", body={
            "device_id": zero.device_id, "xuid": zero.xuid, "public_key": zero.public_key},
            expected=400)
        # The same legitimate device can use either case; free text is not an ID.
        alias = copy.copy(self.host)
        alias.xuid = impostor.xuid
        alias.player_name = impostor.xuid
        enroll(self.api, alias)
        row = self.api.request("POST", "/api/v2/stats/read", alias.access_token,
                               {"xuids": [impostor.xuid], "view_id": "0x0000006d",
                                "stat_ids": []})["rows"][0]
        assert row["xuid"] == self.host.xuid and row["player_name"] == alias.player_name
        return {"case_alias_rejected": True, "storage_owner_preserved": True,
                "same_device_case_accepted": True, "free_text_preserved": True}

    def generated_stats_and_ranked_lifecycle(self):
        session = self.create([self.host, self.peer], ranked=True)
        cash = {"0x2000000d": {"type": "i64", "value": 5000}}
        self.stat(session, cash, expected=409)
        self.modify(session, expected=409, lifecycle_state=2)
        # Exact property IDs/types emitted by sub_829F1D88 through sub_82A373B8.
        platform = {"0x1000800a": {"type": "i32", "value": 7},
                    "0x1000800b": {"type": "i32", "value": 2}}
        accepted, _ = self.stat(session, platform, view=0xFFFF0000)
        self.api.request("POST", "/api/v2/stats/writes", self.host.access_token,
                         accepted, expected=204)
        assert self.current(session)["platform_stat_reports"][self.host.xuid] == platform
        self.stat(session, {"0x1000800a": platform["0x1000800a"]}, view=0xFFFF0000,
                  expected=400)
        malformed = copy.deepcopy(platform)
        malformed["0x1000800a"] = {"type": "i64", "value": 7}
        self.stat(session, malformed, view=0xFFFF0000, expected=400)
        self.stat(session, platform, view=0xFFFF0001, expected=400)
        session = self.register(session, [self.host, self.peer])
        self.stat(session, cash, expected=409)  # Registration is not match start.
        session = self.modify(session, lifecycle_state=2)
        accepted, _ = self.stat(session, cash)
        self.api.request("POST", "/api/v2/stats/writes", self.host.access_token,
                         accepted, expected=204)
        progression = self.api.request("GET", "/api/v2/progression/" + self.host.xuid,
                                       self.host.access_token)
        assert progression["cash"] == 5000, progression
        self.modify(session, expected=409, lifecycle_state=0, state="lobby")
        self.modify(session, expected=409, flags=0x2E, ranked=False)
        self.stat(session, cash, target=self.other.xuid, expected=403)
        session = self.modify(session, lifecycle_state=3)
        self.stat(session, {"0x2000000d": {"type": "i64", "value": 1}})
        unranked = self.create()
        before = self.api.request("GET", "/api/v2/progression/" + self.host.xuid,
                                  self.host.access_token)["cash"]
        self.stat(unranked, cash)
        after = self.api.request("GET", "/api/v2/progression/" + self.host.xuid,
                                 self.host.access_token)["cash"]
        assert after == before
        return {"generated_report_accepted": True, "unknown_reports_rejected": True,
                "prematch_cash_rejected": True, "registered_match_cash_committed_once": True,
                "reporting_phase_supported": True, "unranked_cash_unchanged": True}

    def migration_transport(self):
        session = self.create([self.host, self.peer])
        routes = {who.xuid: self.route(session, who) for who in (self.host, self.peer)}
        payload = base64url(b"old-session-packet")
        def send(who, target, contents):
            return self.api.request("POST", "/api/v2/relay/datagrams", who.access_token,
                                    {"datagrams": [{"source_port": 37000,
                                     "destination_port": 37000,
                                     "destination_ipv4": routes[target.xuid]["virtual_ipv4"],
                                     "payload": contents}]}, expected=204)
        send(self.host, self.peer, payload)
        replacement = copy.deepcopy(session)
        self.ordinal += 1
        replacement.update(session_id=wire_hex(self.ordinal, 16),
                           host_xuid=self.peer.xuid, host_machine_id=self.peer.machine_id,
                           nonce=wire_hex(self.ordinal + 100, 16), exchange_key="0x" + "ef" * 16)
        replacement["members"] = [m for m in replacement["members"] if m["xuid"] == self.peer.xuid]
        migrated = self.api.request("POST", "/api/v2/sessions/" + session["session_id"] + "/migration",
                                    self.peer.access_token,
                                    {"expected_revision": session["revision"],
                                     "expected_host_epoch": session["host_epoch"],
                                     "replacement": replacement})
        assert all(m["xuid"] != self.host.xuid for m in migrated["members"])
        send(self.host, self.peer, payload)
        assert self.api.request("GET", "/api/v2/relay/datagrams?local_port=37000&wait_ms=0",
                                self.peer.access_token)["datagrams"] == []
        self.api.request("GET", "/api/v2/relay/datagrams?local_port=37000&wait_ms=0",
                         self.host.access_token, expected=404)
        self.api.request("POST", "/api/v2/relay/routes", self.host.access_token,
                         {"session_id": migrated["session_id"], "local_port": 37000}, expected=403)
        # Binding the surviving socket to another session invalidates an older close.
        rebound = self.create([self.peer])
        self.route(rebound, self.peer)
        self.api.request("DELETE", "/api/v2/relay/routes", self.peer.access_token,
                         {"session_id": migrated["session_id"], "local_port": 37000}, expected=409)
        self.api.request("GET", "/api/v2/relay/datagrams?local_port=37000&wait_ms=0",
                         self.peer.access_token)
        return {"removed_host_send_receive_revoked": True, "old_queue_discarded": True,
                "rebound_route_survives_stale_close": True}

    def qos(self):
        session = self.create([self.host, self.peer])
        self.route(session, self.host)
        self.route(session, self.peer)
        update = {"session_id": session["session_id"], "exchange_key": session["exchange_key"],
                  "enabled": True, "title_data": base64url(bytes(12))}
        self.api.request("PUT", "/api/v2/qos/listeners", self.host.access_token, update, expected=204)
        index = 0
        def lookup_body():
            nonlocal index
            index += 1
            return {"targets": [{"session_id": session["session_id"],
                     "exchange_key": session["exchange_key"], "challenge": "0x" + f"{index:032x}"}]}
        unacknowledged = self.api.request("POST", "/api/v2/qos/lookup", self.peer.access_token,
                                         lookup_body())
        assert not unacknowledged["results"][0]["reachable"], unacknowledged
        with concurrent.futures.ThreadPoolExecutor(max_workers=1) as executor:
            requested = lookup_body()
            pending = executor.submit(self.api.request, "POST", "/api/v2/qos/lookup",
                                      self.peer.access_token, requested)
            probes = self.api.request("GET", "/api/v3/qos/probes?wait_ms=1000",
                                      self.host.access_token)["probes"]
            probe = next(p for p in probes if p["challenge"] == requested["targets"][0]["challenge"])
            ack = {"probe_id": probe["probe_id"], "challenge": probe["challenge"]}
            self.api.request("POST", "/api/v3/qos/ack", self.other.access_token, ack, expected=403)
            invalid = dict(ack, challenge="0x" + "ff" * 16)
            self.api.request("POST", "/api/v3/qos/ack", self.host.access_token, invalid, expected=403)
            self.api.request("POST", "/api/v3/qos/ack", self.host.access_token, ack, expected=204)
            self.api.request("POST", "/api/v3/qos/ack", self.host.access_token, ack, expected=204)
            measured = pending.result(timeout=6)
        result = measured["results"][0]
        assert result["reachable"] and result["probes_recv"] == 1, measured
        assert result["measurement"] == "relay-host-ack-v1"
        assert result["challenge"] == ack["challenge"]
        assert result["relay_host_microseconds"] <= measured["server_elapsed_microseconds"]
        with concurrent.futures.ThreadPoolExecutor(max_workers=1) as executor:
            requested = lookup_body()
            pending = executor.submit(self.api.request, "POST", "/api/v2/qos/lookup",
                                      self.peer.access_token, requested)
            probes = self.api.request("GET", "/api/v3/qos/probes?wait_ms=1000",
                                      self.host.access_token)["probes"]
            probe = next(p for p in probes if p["challenge"] == requested["targets"][0]["challenge"])
            self.api.request("PUT", "/api/v2/qos/listeners", self.host.access_token,
                             dict(update, enabled=False), expected=204)
            self.api.request("POST", "/api/v3/qos/ack", self.host.access_token,
                             {"probe_id": probe["probe_id"], "challenge": probe["challenge"]}, expected=403)
            assert not pending.result(timeout=6)["results"][0]["reachable"]
        return {"no_host_ack_unreachable": True, "authenticated_host_ack_required": True,
                "challenge_and_generation_checked": True, "duplicate_ack_idempotent": True,
                "measurement": result["measurement"]}

    def subscriptions(self):
        # More legitimate clients than the production default request-worker count.
        identities = [Identity(0xA10000 + i, "review-subscriber-" + str(i)) for i in range(24)]
        for identity in identities:
            enroll(self.api, identity)
        barrier = threading.Barrier(len(identities) + 1)
        def poll(identity):
            barrier.wait()
            return self.api.raw("GET", "/api/v2/events?wait_ms=1800", identity.access_token)
        with concurrent.futures.ThreadPoolExecutor(max_workers=len(identities)) as pool:
            futures = [pool.submit(poll, identity) for identity in identities]
            barrier.wait()
            time.sleep(0.2)
            pending = sum(not f.done() for f in futures)
            started = time.monotonic()
            self.api.request("GET", "/health/live")
            self.api.request("GET", "/health/ready")
            self.api.request("POST", "/api/v2/sessions/search", self.host.access_token,
                             {"maximum_results": 10, "title_id": "0x545407f2", "media_id": "0x00000000",
                              "title_version": "0x00000001", "protocol_version": 2})
            elapsed = time.monotonic() - started
            assert pending > 16, pending
            assert elapsed < 1.0, elapsed
            # One identity cannot open duplicate subscriptions.
            self.api.request("GET", "/api/v2/events?wait_ms=1000", identities[0].access_token,
                             expected=429, error_code="poll_limit")
            replies = [f.result(timeout=5) for f in futures]
            assert all(status == 200 for status, _ in replies), replies
        return {"concurrent_subscribers": len(identities), "pending_at_probe": pending,
                "health_and_control_seconds": elapsed, "duplicate_subscription_rejected": True}


def run(binary: Path, output: Path | None):
    results = []
    with tempfile.TemporaryDirectory(prefix="libserver-reliability-") as directory:
        state = Path(directory) / "state"
        server = ServerProcess(str(binary), str(state), [])
        try:
            api = Api(server.port)
            identities = [Identity(0xAABB + index, "review-identity-" + str(index))
                          for index in range(3)]
            for identity in identities:
                enroll(api, identity)
            suite = Suite(api, identities)
            for name in ("identities", "generated_stats_and_ranked_lifecycle", "migration_transport",
                         "qos", "subscriptions"):
                suite.run(name, getattr(suite, name))
            results.extend(suite.results)
        finally:
            server.stop()
            if output:
                (output / "server-regression-process.log").write_text(server.diagnostics())
            server.close()
        # Refuse a legacy snapshot that binds two device keys to one numeric XUID.
        collision = Path(directory) / "collision"
        shutil.copytree(state, collision)
        path = collision / "state.json"
        content = json.loads(path.read_text())
        devices = content["payload"]["devices"]
        owner = identities[0]
        duplicate = dict(devices[owner.device_id])
        duplicate["xuid"] = "0x" + owner.xuid[2:].upper()
        duplicate["public_key"] = Identity(0xCE01, "collision").public_key
        devices["new-collision-device"] = duplicate
        path.write_text(json.dumps(content))
        before = path.read_bytes()
        process = subprocess.run([str(binary), "--listen", "127.0.0.1", "--port", str(reserve_port()),
                                  "--data-dir", str(collision)], capture_output=True,
                                 text=True, timeout=10)
        passed = process.returncode != 0 and path.read_bytes() == before
        # Initialization must fail payload validation, not merely reject a command-line option.
        passed = passed and "durable payload validation failed" in process.stderr
        results.append({"test": "legacy_identity_collision", "passed": passed,
                        "exit": process.returncode, "diagnostic": process.stderr.strip()})
    report = {"tests": results, "passed": all(r["passed"] for r in results)}
    if output:
        (output / "server-reliability-results.json").write_text(json.dumps(report, indent=2) + "\n")
    print(json.dumps(report, indent=2))
    return 0 if report["passed"] else 1


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("server", type=Path)
    parser.add_argument("--output", type=Path)
    arguments = parser.parse_args()
    if arguments.output:
        arguments.output.mkdir(parents=True, exist_ok=True)
    raise SystemExit(run(arguments.server.resolve(), arguments.output))
