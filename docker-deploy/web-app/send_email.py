import psycopg2
import psycopg2.extras
import smtplib


def check_delivery_status():
    # Connect to the database
    conn = psycopg2.connect(
        host="db",
        database="postgres",
        user="postgres",
        password="postgres"
    )

    # Create a cursor object
    cursor = conn.cursor(cursor_factory=psycopg2.extras.NamedTupleCursor)

    # Execute a query
    cursor.execute("""
    SELECT u.email, p.package_status, p.package_id, p.description
    FROM ups_package AS p
    JOIN auth_user AS u ON u.id = p.user_id
    WHERE p.package_status = 'delivered' AND p.email_sent = false AND p.user_id IS NOT NULL;
    """)

    # Fetch the results
    results = cursor.fetchall()

    # Check if there are any delivered packages
    if len(results) > 0:
        print("Yes, there are delivered packages:")
        for row in results:
            email(row.email, row.package_id, row.description)
            cursor.execute("UPDATE ups_package SET email_sent = true WHERE package_id = " + str(row.package_id))
            conn.commit()
    #else:
        #print("No delivered packages found.")

    # Close the cursor and connection
    cursor.close()
    conn.close()



def email(receiver_email,package_id,description):
    sender_email = "rideece568duke@outlook.com"
    password = "Asdfghjklqwertyuiop"

    message = f"Subject: From UPS\n\nYour package (package_id = {package_id}, content={description}) has been delivered."


    # Connect to SMTP server
    server = smtplib.SMTP('smtp.office365.com', 587)
    server.starttls()
    server.login(sender_email, password)

    # Send email
    server.sendmail(sender_email, receiver_email, message)

    # Close SMTP server
    server.quit()

    print("Email sent successfully!")

count = 0
while True:
    check_delivery_status()
    count += 1