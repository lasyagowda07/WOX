from twilio.rest import Client

# Your Twilio account SID and auth token
account_sid = 'AC75de8e517e2dc22716b57627b124545d'
auth_token = '4c50b52b68cb6c70835b9a9b16631455'

# Create a client object
client = Client(account_sid, auth_token)

# The phone number you want to send the SMS to, including the country code
to_number = '+917483952159'

# The phone number you purchased from Twilio, including the country code
from_number = '+13184963674'

# The message you want to send
message = 'Please help me.'

# Send the SMS message
client.messages.create(
    to=to_number,
    from_=from_number,
    body=message
)





