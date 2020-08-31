First, create a virtualenv:

```console
$ python3 -mvenv .venv
$ source .venv/bin/activate
$ pip install -r requirements.txt
```

Open the S3 dashboard and create a new bucket. Then, go to the IAM dashboard
and create a policy:

```json
{
    "Version": "2012-10-17",
    "Statement": [
        {
            "Effect": "Allow",
            "Action": ["s3:PutObject", "s3:GetObject", "s3:DeleteObject"],
            "Resource:": ["arn:aws:s3:::YOUR_BUCKET/*"]
        }
    ]
}
```

Next, create a group and attach the policy to the group. Create a user with
program access and let the user join the group. Take note of the access key ID
and the access secret.

Set AWS env vars and run the script:

```console
$ export AWS_ACCESS_KEY_ID=...
$ export AWS_SECRET_ACCESS_KEY=...
$ export AWS_DEFAULT_REGION=...
$ ./main.py YOUR_BUCKET
Get: (none)
Put done.
```

The script should have created an object named "default.json" in the bucket.
Repeatedly running the script updates the object with new content:

```console
$ ./main.py YOUR_BUCKET
Get: {'date': '2020-08-31 16:47:57'}
Put done.
```
