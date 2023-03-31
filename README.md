# Chatlis

A chatroom made in C++ with the Qt librairy.

An expanded and updated version of Qt's example project : https://doc.qt.io/qt-6/qtnetwork-network-chat-example.html

# Using Chatlis w/SSL (v1.0.0 and up)

The server mainainer needs to generate an SSL certificate that is signed with a CA certificate. 

## Generate the certificates

The following example will do 3 things:

- Generate a CA certificate that will be used by clients to verify that they are connecting to the right server.
- Generate a Csr+Key pair that will be used by the server to negotiate SSL connections with clients.
- Generate a signed certificate using the Csr and the CA certificate to allow clients to validate the server's identity.

1. Ceate the CA certificate:

```sh
openssl genrsa -out rootCA.key 2048
openssl req -x509 -new -key rootCA.key -days 3650 -out rootCA.pem \
-subj '/C=AA/ST=AA/L=AA/O=AA Ltd/OU=AA/CN=AA/emailAddress=aa@aa.com'
```

2. Create the server csr and key pair:

```sh
openssl genrsa -out server.key 2048
openssl req -new -key server.key -out server.csr \
-subj '/C=BB/ST=BB/L=BB/O=BB Ltd/OU=BB/CN=BB/emailAddress=bb@bb.com'
```

3. Generate a signed certificate

```sh
openssl x509 -req -days 365 -CA rootCA.pem -CAkey rootCA.key \
-CAcreateserial -CAserial serial -in server.csr -out server.pem
```

## Use the certifactes

You should now have these files:

- rootCA.key
- rootCA.pem
- server.key
- server.csr
- server.pem

You should now delete the following files for security:
- rootCA.key
- server.

*(they serve no purpose to the applciation)*

### On the server side

You should put `server.key` and `server.pem` in your server's `place-your-certificate-key-pair-here` folder.

### On the client side

Each user wanting to connect to your server should get a copy of **YOUR** `rootCA.pem` file in their client's `place-your-ca-certificates-here` folder.

## You're done
This should be everything you need to get the server and clients to talk to each other over SSL using a secure protocol! 👍