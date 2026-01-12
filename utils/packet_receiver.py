import RNS
import time
import threading
import sys
known_destination_hash = None

class ExampleAnnounceHandler:
    # The initialisation method takes the optional
    # aspect_filter argument. If aspect_filter is set to
    # None, all announces will be passed to the instance.
    # If only some announces are wanted, it can be set to
    # an aspect string.
    def __init__(self, aspect_filter=None):
        self.aspect_filter = aspect_filter

    # This method will be called by Reticulums Transport
    # system when an announce arrives that matches the
    # configured aspect filter. Filters must be specific,
    # and cannot use wildcards.
    def received_announce(self, destination_hash, announced_identity, app_data):
        global known_destination_hash
        RNS.log(
            "Received an announce from "+
            RNS.prettyhexrep(destination_hash)
        )

        if app_data:
            RNS.log(
                "The announce contained the following app data: "+
                app_data.decode("utf-8")
            )
        with known_destination_lock:
            known_destination_hash = destination_hash
            print("known_destination_hash is set")
            
            
# --------------------------------------------------
# Connect to rnsd
# --------------------------------------------------
RNS.Reticulum(require_shared_instance=False)

# --------------------------------------------------
# Identity and destination
# --------------------------------------------------
identity = RNS.Identity()

destination = RNS.Destination(
    identity,
    RNS.Destination.IN,
    RNS.Destination.SINGLE,
    "example",
    "packet"
)

print("My destination hash:", destination.hash.hex())

# --------------------------------------------------
# Storage for first known remote destination
# --------------------------------------------------

known_destination_lock = threading.Lock()

# --------------------------------------------------
# Announce receive handler
# --------------------------------------------------
announce_handler = ExampleAnnounceHandler()

# We register the announce handler with Reticulum
RNS.Transport.register_announce_handler(announce_handler)


# --------------------------------------------------
# Packet receive handler
# --------------------------------------------------
def packet_received(data , packet ):
    sender_hash = packet.destination.hash

    #try:
    #    message = packet.data.decode()
    #except Exception:
     #   message = str(packet.data)

    #print(f"\n[RX] {sender_hash.hex()}: {message}")
    print("Received data: "+data.decode("utf-8")+"\r\n> ", end="")
    sys.stdout.flush()

destination.set_packet_callback(packet_received)

# --------------------------------------------------
# Announce function
# --------------------------------------------------
def announce():
    destination.announce()
    print("[*] Announcement sent")

# --------------------------------------------------
# Send function
# --------------------------------------------------
def send_message(message):
    global known_destination_hash
    if message != "":
        data    = message.encode("utf-8")
        packet  = RNS.Packet(known_destination_hash, data)
        packet.send()
   
    print("[TX] Sent message")

# --------------------------------------------------
# Command loop
# --------------------------------------------------
def command_loop():
    print("\nCommands:")
    print("  a = announce")
    print("  s = send message")
    print("  e = exit\n")

    while True:
        try:
            cmd = input("> ").strip().lower()

            if cmd == "e":
                print("Exiting.")
                sys.exit(0)

            elif cmd == "a":
                announce()

            elif cmd == "s":
                msg = input("Message to send: ")
                send_message(msg)

            else:
                print("Unknown command")

        except KeyboardInterrupt:
            print("\nExiting.")
            sys.exit(0)

# --------------------------------------------------
# Start command loop
# --------------------------------------------------
command_loop()
