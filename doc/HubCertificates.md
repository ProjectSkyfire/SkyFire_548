# Hub certificate management

The hub can act as the cluster certificate authority for auth, world, map,
character and chat daemons. It provisions managed daemons before launch,
accepts one-time enrollment tokens from remote machines, renews certificates
before expiry, and publishes signed certificate revocation lists (CRLs).
This is opt-in: existing manual TLS configurations continue working with
`Hub.PKI.Enable = 0` and empty daemon CRL settings.

## Requirements and installation

Build matching hub, auth, world and chat binaries and run CMake INSTALL to ship
the updated web, map and character assets. Python services and the remote
enrollment utility require Python 3.11+ and `cryptography==50.0.1`; install the
updated map/character `requirements.txt` in their environment. Native signing
uses the project's existing OpenSSL dependency.

The authority store contains the CA private key, certificate inventory, revoked
serials, blocked identities and hashes of enrollment tokens. Back up the entire
store securely. Never restore only its CRL counter: clients reject older CRLs.
Only one hub process may open a store. Daemons must not receive the CA private
key. Managed processes run under the hub's OS identity; separate service users
need explicitly provisioned access to their own leaf key, certificate and CA.
Restrict the authority directory to the hub operator/service account.

## New cluster

Configure `Hub.Cluster.Enable = 1` and:

```ini
Hub.PKI.Enable = 1
Hub.PKI.Directory = "pki"
Hub.PKI.CA = "pki/ca.pem"
Hub.PKI.PrivateKey = "pki/ca-key.pem"
Hub.PKI.ImportDirectory = ""
Hub.PKI.HubNames = "hub.example.org,192.0.2.10"
Hub.PKI.ValidDays = 90
Hub.PKI.RenewDays = 14
```

PKI paths are relative to the hub configuration directory. The hub creates a
new CA only when both CA and key are missing and no existing cluster CA file
is configured. With PKI enabled it uses its managed hub listener certificate;
include every hostname/IP that daemons use to reach the hub in `HubNames`
before initial creation. Changing names later requires a planned certificate
replacement; changing the setting alone does not rewrite existing profiles.

Configure each managed daemon's normal node key, advertised address, hub
endpoint and access allowlists. On Start, the hub issues credentials and updates
certificate/key/CA/CRL paths in its configuration, retaining a `.pre-pki` backup.
Node keys for managed PKI use 1–64 letters, digits, underscores or hyphens,
beginning with a letter/digit; Windows reserved device names are excluded.
Provisioning does not grant realms, database ownership or world allowlists.

## Adopt an existing cluster

Plan a maintenance restart and preserve current TLS/configuration backups.
Set `Hub.PKI.CA` and `Hub.PKI.PrivateKey` to the **matching existing CA and
private key**. The private key must be readable without an interactive password
and protected with filesystem permissions. Set `Hub.PKI.ImportDirectory` to a
directory containing the current public leaf certificates (`.pem`/`.crt`).
The hub validates and archives certificates signed by this CA so existing
identities can connect and their old serials can later be revoked. This directory
is read at startup, not recursively. Do not omit remote daemon certificates.

Start the hub, then start managed daemons through it and enroll remote daemons.
Verify every daemon has a CRL path and matching current code before relying on
revocation. A legacy peer without CRL enforcement can still accept a revoked
certificate directly, even though the updated hub rejects it. Importing a CA
does not update other machines' code or trust settings automatically.

## Remote enrollment

In the local hub web console, open **Certificates** and enter the approved node
key, role and comma-separated DNS/IP names. Generate a token. It is shown once,
expires in ten minutes and can be consumed only once. Creating another token
for a node invalidates its previous pending token. Revocation invalidates pending
tokens. Use the loopback console or an authenticated TLS-protected tunnel to it.

Copy the public CA to the remote machine through a trusted channel. Use the
installed `mapserver/cluster_certificates.py` (also shipped with characterserver):

```text
python cluster_certificates.py --host hub.example.org --ca cluster-ca.pem --node world-remote --output tls/world-remote --config worldserver.conf
```

Add `--python` for map/character TOML configurations. `--port` defaults to 9100.
The utility prompts for the token without echoing it, generates the private key
locally, sends a signed certificate request, verifies the response against the
pinned CA and local key, and populates TLS paths in the existing configuration.
The configured node key must match the approved node. Configure the hub endpoint
and service allowlists separately before starting the daemon.

No client certificate is needed for this one enrollment request. All other
cluster requests still require a valid issued/imported certificate. The token
never replaces verification of the hub's server certificate. Failed or uncertain
enrollment may consume the token; issue another instead of reusing it.

## Renewal, revocation and regeneration

Managed leaf certificates renew hourly when within the configured renewal
window, including expired local certificates. Native/Python remote daemons
check hourly within fourteen days of expiration; the hub enforces its configured
window. Keep `RenewDays = 14` for the default behavior. Renewal retains the leaf
key and leaves the previous certificate valid until its normal expiry, allowing
connections to finish. Updated listeners and clients reload credentials for new
connections. Renewing a certificate is not a private-key rotation.

**Revoke** blocks the node at the hub, closes its hub connection, marks every
known certificate serial revoked, and distributes the signed CRL on daemon
heartbeats. Map/chat requests and character RPCs check peer revocation. CRLs
refresh hourly, expire after 24 hours and fail closed when absent/stale if a CRL
path is configured. Propagation normally takes up to one heartbeat; it is not
instant across disconnected machines. Existing map transfers may finish.

**Regenerate** issues a new managed certificate and revokes previous serials.
Remote nodes use a new enrollment token; successful re-enrollment revokes their
previous archived certificates. Revoked nodes never automatically renew or
reenroll. Remote nodes offline past expiry require a new operator-issued token.

Revocation/regeneration deliberately interrupts connections using the old
certificate. Use a maintenance shutdown before operating on a live character
service or world identity: revocation is not a character-save mechanism or a
replacement for the realm ownership fence. Normal scheduled renewal does not
revoke active connections. For suspected private-key compromise, revoke, replace
the leaf key through a planned reprovision/enrollment, and validate before reuse.

The CA is valid for ten years; CA rotation is a separate planned trust migration.
The store is bounded to 128 managed/enrolled profiles and 1,024 unexpired leaf
certificates. Expired issuance records are pruned; public expired profiles remain
visible. An exhausted bound fails issuance without weakening authentication.

## Local console

```text
certificates list
certificates token world-remote 2 world.example.org,192.0.2.20
certificates revoke world-remote
certificates regenerate chat-primary
```

Roles: 1 auth, 2 world, 3 map, 4 character, 5 chat. Certificate actions require
the local console or a full administrator web session with CSRF validation.
Audit entries contain action, identity and actor; they do not contain private
keys or tokens. Do not record the console token output in shared transcripts.

## Validation

`src/tests/cluster_certificates_test.py` exercises signed CSR generation,
certificate identity/key checks, CRL signatures/expiry/rollback protection and
revocation of existing connections using temporary test keys only.
`hub_certificate_tests` exercises the native authority, one-time token use,
revocation persistence, reenrollment, provisioning and regeneration. Run it
after the native build, then validate managed and remote enrollment against a
disposable cluster before enabling PKI on a running deployment.
