document.addEventListener('DOMContentLoaded', function() {

  // Use buttons to toggle between views
  document.querySelector('#inbox').addEventListener('click', () => load_mailbox('inbox'));
  document.querySelector('#sent').addEventListener('click', () => load_mailbox('sent'));
  document.querySelector('#archived').addEventListener('click', () => load_mailbox('archive'));
  document.querySelector('#compose').addEventListener('click', compose_email);

  // By default, load the inbox
  load_mailbox('inbox');
  console.log('loaded')

  document.querySelector('#compose-form').onsubmit = () => send_email();

  document.addEventListener('click', event => {
    if (event.target.id === 'archive') {
      let element = event.target;
      archive_email(element.dataset.id);
    }
    else if (event.target.id === 'reply') {
      reply_email(event.target.dataset.id);
    }
    else {
      let element = event.target.closest('.email, .read-email');
      if (element) {
        view_email(element.dataset.id);
      }
    }
  })
});


function send_email() {

  fetch('/emails', {
    method: 'POST',
    body: JSON.stringify({
      recipients: document.querySelector('#compose-recipients').value,
      subject: document.querySelector('#compose-subject').value,
      body: document.querySelector('#compose-body').value
    })
  })
  .then(response => response.json())
  .then(result => {
    console.log(result);
  });
  
  load_mailbox('sent')
  return false;
}

function compose_email() {

  // Show compose view and hide other views
  document.querySelector('#emails-view').style.display = 'none';
  document.querySelector('#view-email').style.display = 'none';
  document.querySelector('#compose-view').style.display = 'block';

  // Clear out composition fields
  document.querySelector('#compose-recipients').value = '';
  document.querySelector('#compose-subject').value = '';
  document.querySelector('#compose-body').value = '';
}

function load_mailbox(mailbox) {
  
  // Show the mailbox and hide other views
  document.querySelector('#emails-view').style.display = 'block';
  document.querySelector('#compose-view').style.display = 'none';
  document.querySelector('#view-email').style.display = 'none';

  // Show the mailbox name
  document.querySelector('#emails-view').innerHTML = `<h2>${mailbox.charAt(0).toUpperCase() + mailbox.slice(1)}</h2>`;
  const emailbox = document.createElement('div');
  emailbox.id = "emailbox"
  document.querySelector('#emails-view').append(emailbox)
  //fetch emails and display
  fetch(`/emails/${mailbox}`)
  .then(response => response.json())
  .then(emails => {
    console.log(emails);
    emails.forEach(email => add_email(email))
  }); 
}

function add_email(email) {
  console.log("haha");
  const mail = document.createElement('div');
  if (email.read){
    mail.className = 'read-email'
  }
  else {
    mail.className = 'email';
  };
  mail.dataset.id = email["id"];
  mail.innerHTML = 
  `<div class="email-left">
      <strong class="sender">${email["sender"]}</strong>
      ${email["subject"]}
  </div>
  <div class="email-right">
      <div class="date">${email["timestamp"]}</div>
  </div>`;
  console.log(mail);
  document.querySelector('#emailbox').append(mail); 
}

function view_email(email_id) {
  document.querySelector('#emails-view').style.display = 'none';
  document.querySelector('#compose-view').style.display = 'none';
  document.querySelector('#view-email').style.display = 'block';

  fetch(`/emails/${parseInt(email_id)}`)
  .then(response => response.json())
  .then(email => {
    document.querySelector('#view-subject').innerHTML = email.subject;
    document.querySelector('#view-date').innerHTML = email.timestamp;
    document.querySelector('#view-sender').innerHTML = email.sender;
    document.querySelector('#view-content').innerHTML = email.body;
    document.querySelector('#reply').dataset.id = email.id;
    const archive = document.querySelector('#archive');
    if (email.archived){
      archive.innerHTML = 'Unarchive';
    }
    else {
      archive.innerHTML = 'Archive';
    };
    archive.dataset.id = email.id;

    const user_email = document.querySelector('#useremail').innerHTML;
    let recipients = email.recipients;
    for (let i = 0; i < recipients.length; i++) {
      if (recipients[i] === user_email) {
        recipients[i] = 'me'
      }
    }
    const view_recipients = "to " + recipients.join(', ')

    document.querySelector('#view-recipients').innerHTML = view_recipients;
    
    fetch(`/emails/${parseInt(email_id)}`, {
      method: 'PUT',
      body: JSON.stringify({
          read: true
      })
    })
  })
}

function archive_email(email_id) {
  fetch(`/emails/${parseInt(email_id)}`)
  .then(response => response.json())
  .then(email => {
      // Print email
      console.log(email);

      if (email.archived){
        fetch(`/emails/${email_id}`, {
          method: 'PUT',
          body: JSON.stringify({
              archived: false
          })
        })
      }
      else {
        fetch(`/emails/${email_id}`, {
          method: 'PUT',
          body: JSON.stringify({
              archived: true
          })
        })
      }
  });
  location.reload();
  load_mailbox('inbox');
}

function reply_email(email_id) {
  fetch(`/emails/${parseInt(email_id)}`)
  .then(response => response.json())
  .then(email => {
    console.log(email);
    // Show compose view and hide other views
    document.querySelector('#emails-view').style.display = 'none';
    document.querySelector('#view-email').style.display = 'none';
    document.querySelector('#compose-view').style.display = 'block';

    document.querySelector('#compose-recipients').value = email.sender;

    if (email.subject.indexOf('Re: ') !== 0) {
      document.querySelector('#compose-subject').value = 'Re: '+email.subject;
    }
    else {
      document.querySelector('#compose-subject').value = email.subject;
    }
    
    document.querySelector('#compose-body').value = 'On '+ email.timestamp + ', ' + email.sender + " wrote: \n\n" + email.body + "\n\n +--------------------+ \n\n";
  });
}