document.addEventListener('DOMContentLoaded', function() {

    var indexUrl = document.getElementById('indexLink').href;

    document.addEventListener('submit', event => {
        let element = event.target.closest('.editForm')
        if (element) {
            save_post(element);
            event.preventDefault();
        }
    })
    if (document.querySelector('#currentUsername')){
        document.addEventListener('click', event => {
            let element = event.target;
            if (element.classList) {
                if (element.classList.contains("bi-suit-heart")) {
                    element.classList.remove("bi-suit-heart");
                    element.classList.add("bi-suit-heart-fill");
                    like_post(element.parentElement.parentElement);
                }
                else if (element.classList.contains("bi-suit-heart-fill")) {
                    element.classList.remove("bi-suit-heart-fill");
                    element.classList.add("bi-suit-heart");
                    unlike_post(element.parentElement.parentElement);
                }
            }
        })
    }
    
    if (window.location.href === indexUrl) {
        if (document.querySelector('#makePost')){
            document.querySelector('#makePost').addEventListener('click', new_post);
        }
        document.querySelector('#PostForm').onsubmit = () => submit_post();
        document.querySelector('#closeButton').addEventListener('click', close_button);
        document.querySelector('#allPosts').addEventListener('click', () => find_posts('all'))
        if (document.querySelector('#userFollowing')) {
            document.querySelector('#userFollowing').addEventListener('click', () => {
                count_pages("following");
            })
        }
        
        count_pages('all');

        document.addEventListener('click', event => {
            let element = event.target.closest('.page-number, #previouspage, #nextpage')
            if (element) {
                if (element.classList.contains('page-number')) {
                    find_posts(document.getElementById('currentPosts').dataset.current, element.dataset.number, document.getElementById('pagebar').dataset.count);
                }
                else if (element.id === 'previouspage' &&  !event.target.classList.contains('disabled')){
                    find_posts(document.getElementById('currentPosts').dataset.current, parseInt(document.getElementById('currentPosts').dataset.page)-1, document.getElementById('pagebar').dataset.count);
                }
                else if (element.id === 'nextpage' &&  !event.target.classList.contains('disabled')) {
                    find_posts(document.getElementById('currentPosts').dataset.current, parseInt(document.getElementById('currentPosts').dataset.page)+1, document.getElementById('pagebar').dataset.count);
                }
            }
        })
    }
    else if (window.location.href === (indexUrl + '#')) {
        window.location.href = indexUrl;
    }
    else {
       if (document.getElementById('profileposts')){
        var id = document.getElementById('profileposts').dataset.id;
        console.log(id)
        count_pages(id);
       }

       document.getElementById('backButton').addEventListener('mouseover', () => {
            document.getElementById('backButton').classList.remove('bi-arrow-left-square');
            document.getElementById('backButton').classList.add('bi-arrow-left-square-fill');   
       })
       document.getElementById('backButton').addEventListener('mouseout', () => {
            document.getElementById('backButton').classList.remove('bi-arrow-left-square-fill');
            document.getElementById('backButton').classList.add('bi-arrow-left-square');
        })
        document.addEventListener('click', event => {
            let element = event.target.closest('.page-number, #previouspage, #nextpage')
            if (element) {
                console.log(element);
                if (element.classList.contains('page-number')) {
                    profile_display(id, element.dataset.number, document.getElementById('pagebar').dataset.count);
                }
                else if (element.id === 'previouspage' &&  !event.target.classList.contains('disabled')){
                    profile_display(id, parseInt(document.getElementById('profileposts').dataset.page)-1, document.getElementById('pagebar').dataset.count);
                }
                else if (element.id === 'nextpage' &&  !event.target.classList.contains('disabled')) {
                    profile_display(id, parseInt(document.getElementById('profileposts').dataset.page)+1, document.getElementById('pagebar').dataset.count);
                }
            }
        })
        var followButton = document.getElementById("followButton")
        if (followButton) {
            followButton.addEventListener('click', () => {
                if (followButton.dataset.type === 'follow') {
                    follow(followButton.dataset.id)
                }
                else {
                    unfollow(followButton.dataset.id)
                }
            })
        }
    }
    document.addEventListener('click', event => {
        let element = event.target;
        if (element.classList.contains('edit')) {
            edit_post(element.parentElement);
        }
    })
})

function count_pages(box) {
    fetch(`/num_pages/${box}`)
    .then(response => response.json())
    .then(response => {
        console.log(response.num_pages);
        if (response.num_pages > 0) {
            document.getElementById('paginatebar').style.display = 'block'
            var pagebar = document.getElementById('pagebar');
            pagebar.innerHTML = '';
            pagebar.dataset.count = response.num_pages;
            var previous = document.createElement('li');
            previous.className = 'page-item';
            previous.id = 'previouspage'
            previous.innerHTML = `<a class="page-link" href="#" id="previous">
                      <span aria-hidden="true">&laquo;</span>
                    </a>`
            pagebar.append(previous)
            for (var i = 1; i <= response.num_pages; i++) {
                var page = document.createElement('li');
                page.className = "page-item";
                page.classList.add('page-number');
                page.id = `pageno${i}`
                page.dataset.number = i;
                page.innerHTML = `<a class="page-link" href="#" >${i}</a>`;
                pagebar.append(page);
            }
            var next = document.createElement('li');
            next.className = 'page-item';
            next.id = 'nextpage';
            next.innerHTML = `<a class="page-link" href="#" id="next">
                      <span aria-hidden="true">&raquo;</span>
                    </a>`;
            pagebar.append(next);
            var boxInt = parseInt(box);
            if (isNaN(boxInt)) {
                find_posts(box, 1, response.num_pages);
            }
            else {
                profile_display(box, 1, response.num_pages);
            }
            
        }
        else{
            document.getElementById('paginatebar').style.display = 'none'
        }
    })
}

function new_post() {
    console.log("HAHA")
    document.querySelector('#NewPost').style.display = 'block';

    gsap.timeline()
        .to("#NewPost", { 
            duration: 0.5, 
            height: 'auto', 
            padding: '2vh', 
            marginBottom: '2vh',
            onComplete: function() {
                gsap.set("#NewPost", { height: 'auto' });
                document.querySelector('#postcontent').value = ''
            } 
        })
        .to("#NewPost", { 
            duration: 0.5, 
            opacity: 1,
                
        });

}

function submit_post() {
    
    fetch('/compose', {
        method: 'POST',
        body: JSON.stringify({
            content: document.querySelector('#postcontent').value
        })
    })
    .then(response => response.json())
    .then(result => {
        console.log(result)
    });

    gsap.timeline()
        .to("#NewPost", { duration: 0.5, opacity: 0 })
        .to("#NewPost", { 
            duration: 0.5, 
            height: 0, 
            padding: 0, 
            marginBottom: 0, 
            onComplete: function() {
                document.querySelector('#NewPost').style.display = 'none'; 
                console.log("completed animation");
                count_pages("all");
            }
        });
    console.log("HAHA");

    return false;
}

function close_button() {
    gsap.timeline()
        .to("#NewPost", { duration: 0.5, opacity: 0 })
        .to("#NewPost", { 
            duration: 0.5, 
            height: 0, 
            padding: 0, 
            marginBottom: 0, 
            onComplete: function() {
                document.querySelector('#NewPost').style.display = 'none'; 
                console.log("completed animation");
                    }
        });
}

function find_posts(box, page, max) {

    if (parseInt(page) == 1) {
        document.getElementById('previous').tabIndex = -1;
        document.getElementById('previous').parentElement.classList.add("disabled");
    }
    else {
        document.getElementById('previous').tabIndex = 0;
        document.getElementById('previous').parentElement.classList.remove("disabled");
    }

    if (parseInt(page) === parseInt(max)) {
        document.getElementById('next').tabIndex = -1;
        document.getElementById('next').parentElement.classList.add("disabled");
    }
    else {
        document.getElementById('next').tabIndex = 0;
        document.getElementById('next').parentElement.classList.remove("disabled");
    }

    document.getElementById('currentPosts').dataset.current = box;
    document.getElementById('currentPosts').dataset.page = page;
    fetch(`/posts/${box}/${parseInt(page)}`)
    .then(response => response.json())
    .then(posts => {
        console.log(posts);
        document.getElementById('currentPosts').innerHTML = ''
        posts.forEach(post => {
            console.log(post.creator);
            display_post(post);
        })
        console.log("displaypost successful");
    })
    .catch(error => {
        console.log('Error:', error);
    });

    for (var i = 1; i < parseInt(max)+1; i++) {
        if (document.getElementById(`pageno${i}`).classList.contains('active')) {
            document.getElementById(`pageno${i}`).classList.remove('active');
        }
    }

    document.getElementById(`pageno${page}`).classList.add('active');
}

function display_post(post) {
    console.log("displaying post function reached");
    var indexUrl = document.getElementById('indexLink').href;
    const postdiv = document.createElement('div');
    postdiv.className = 'postcontainer';
    postdiv.dataset.id = post.id;
    if (document.getElementById('currentUsername') && document.getElementById('currentUsername').innerHTML === post.creator) {
        postdiv.innerHTML = 
        `<button class="btn btn-outline-dark edit"><i class="bi bi-pencil-square"></i> Edit</button>
        <strong class="postCreator"><a class="creatorRef" href="${indexUrl+"profile/"+post.creatorid}">${post.creator}</a></strong>
        <div class="postContent">${post.body}</div>
        <div class="postDate">${post.timestamp}</div>
        <div class="d-flex align-items-center">
            <i class="bi bi-suit-heart" id="heartButton"></i> <div class="postLikes">${post.likes.length}</div>
        </div>`
    }
    else {
        postdiv.innerHTML = 
        `<strong class="postCreator"><a class="creatorRef" href="${indexUrl+"profile/"+post.creatorid}">${post.creator}</a></strong>
        <div class="postContent">${post.body}</div>
        <div class="postDate">${post.timestamp}</div>
        <div class="d-flex align-items-center">
            <i class="bi bi-suit-heart" id="heartButton"></i> <div class="postLikes">${post.likes.length}</div>
        </div>`;
    }

    if (post.edit) {
        var editText = document.createElement('div')
        editText.className = 'editText'
        editText.innerHTML = 'edited'
        postdiv.append(editText)
    }
    if (document.getElementById('currentUsername') && post.likes.includes(document.getElementById('currentUsername').innerHTML)) {
        var heart = postdiv.querySelector('.bi-suit-heart');
        heart.classList.remove('bi-suit-heart');
        heart.classList.add('bi-suit-heart-fill')
    }
    console.log(postdiv);
    document.getElementById('currentPosts').append(postdiv);
}

function profile_display(id, page, max) {

    if (parseInt(page) == 1) {
        document.getElementById('previous').tabIndex = -1;
        document.getElementById('previous').parentElement.classList.add("disabled");
    }
    else {
        document.getElementById('previous').tabIndex = 0;
        document.getElementById('previous').parentElement.classList.remove("disabled");
    }

    if (parseInt(page) === parseInt(max)) {
        document.getElementById('next').tabIndex = -1;
        document.getElementById('next').parentElement.classList.add("disabled");
    }
    else {
        document.getElementById('next').tabIndex = 0;
        document.getElementById('next').parentElement.classList.remove("disabled");
    }

    document.getElementById('profileposts').dataset.page = page;
    fetch(`/postsprof/${parseInt(id)}/${parseInt(page)}`)
    .then(response => response.json())
    .then(posts => {
        document.getElementById('profileposts').innerHTML = ''
        posts.forEach(post => {
            display_profile(post);
        })
    })
    .catch(error => {
        console.log('Error:', error);
    });

    for (var i = 1; i < parseInt(max)+1; i++) {
        if (document.getElementById(`pageno${i}`).classList.contains('active')) {
            document.getElementById(`pageno${i}`).classList.remove('active');
        }
    }

    document.getElementById(`pageno${page}`).classList.add('active');
}

function display_profile(post) {
    console.log("displaying post function reached");
    var indexUrl = document.getElementById('indexLink').href;
    const postdiv = document.createElement('div');
    postdiv.className = 'postcontainer';
    postdiv.dataset.id = post.id;
    if (document.getElementById('currentUsername') && document.getElementById('currentUsername').innerHTML === post.creator) {
        postdiv.innerHTML = 
        `<button class="btn btn-outline-dark edit"><i class="bi bi-pencil-square"></i> Edit</button>
        <strong class="postCreator"><a class="creatorRef" href="${indexUrl+"profile/"+post.creatorid}">${post.creator}</a></strong>
        <div class="postContent">${post.body}</div>
        <div class="postDate">${post.timestamp}</div>
        <div class="d-flex align-items-center">
            <i class="bi bi-suit-heart" id="heartButton"></i> <div class="postLikes">${post.likes.length}</div>
        </div>`
    }
    else {
        postdiv.innerHTML = 
        `<strong class="postCreator"><a class="creatorRef" href="${indexUrl+"profile/"+post.creatorid}">${post.creator}</a></strong>
        <div class="postContent">${post.body}</div>
        <div class="postDate">${post.timestamp}</div>
        <div class="d-flex align-items-center">
            <i class="bi bi-suit-heart" id="heartButton"></i> <div class="postLikes">${post.likes.length}</div>
        </div>`;
    }
    if (post.edit) {
        var editText = document.createElement('div')
        editText.className = 'editText'
        editText.innerHTML = 'edited'
        postdiv.append(editText)
    }
    if (document.getElementById('currentUsername') && post.likes.includes(document.getElementById('currentUsername').innerHTML)) {
        var heart = postdiv.querySelector('.bi-suit-heart');
        heart.classList.remove('bi-suit-heart');
        heart.classList.add('bi-suit-heart-fill')
    }

    console.log(postdiv);
    document.getElementById('profileposts').append(postdiv);
}

function follow(user_id) {
    fetch(`/follow/${parseInt(user_id)}`, {
        method: 'PUT',
        body: JSON.stringify({
            follow: true
        })
    })
    var followButton = document.getElementById("followButton");
    followButton.classList.remove('btn-outline-primary');
    followButton.classList.add('btn-outline-danger');
    followButton.dataset.type = 'unfollow';
    followButton.innerHTML = '<i class="bi bi-person-dash"></i> Unfollow'
    document.getElementById('followers').innerHTML = `${parseInt(document.getElementById('followers').innerHTML) + 1}`
}

function unfollow(user_id) {
    fetch(`/follow/${parseInt(user_id)}`, {
        method: 'PUT',
        body: JSON.stringify({
            follow: false
        })
    })
    var followButton = document.getElementById("followButton");
    followButton.classList.remove('btn-outline-danger');
    followButton.classList.add('btn-outline-primary');
    followButton.dataset.type = 'follow';
    followButton.innerHTML = '<i class="bi bi-person-add"></i> Follow'
    document.getElementById('followers').innerHTML = `${parseInt(document.getElementById('followers').innerHTML) - 1}`
}

function edit_post(post_div) {
    if (document.getElementById('currentUsername').innerHTML === post_div.querySelector('.creatorRef').innerHTML) {
        var content = post_div.querySelector('.postContent').innerHTML;
        post_div.innerHTML = `<form class="center-form editForm">
            <h2 style="font-family: 'GeoSansBody', Arial, sans-serif;">Edit Echo</h2>
            <textarea class='editcontent' required autofocus name="content"></textarea>
            <input type="submit" class="btn btn-outline-dark btnnew" style="margin-top: 2vh;" value="Save">
        </form>`
        post_div.querySelector('.editcontent').innerHTML = content;
    }
}

function save_post(form) {
    var indexUrl = document.getElementById('indexLink').href;
    var post_id = form.parentElement.dataset.id;
    var body = form.querySelector('.editcontent').value
    fetch(`/edit/${parseInt(post_id)}`, {
        method: 'POST',
        body: JSON.stringify({
            content: body
        })
    })
    .then(response => response.json())
    .then(post => {
        form.parentElement.innerHTML = `<button class="btn btn-outline-dark edit"><i class="bi bi-pencil-square"></i> Edit</button>
        <strong class="postCreator"><a class="creatorRef" href="${indexUrl+"profile/"+post.creatorid}">${post.creator}</a></strong>
        <div class="postContent">${body}</div>
        <div class="postDate">${post.timestamp}</div>
        <div class="d-flex align-items-center">
            <i class="bi bi-suit-heart" id="heartButton"></i> <div class="postLikes">${post.likes.length}</div>
        </div>
        <div class="editText">edited</div>`
    })
    
    return false;
}

function like_post(post) {
    var post_id = post.dataset.id
    fetch(`/like/${parseInt(post_id)}`, {
        method: 'PUT',
        body: JSON.stringify({
            like: true
        })
    })
    post.querySelector('.postLikes').innerHTML = parseInt(post.querySelector('.postLikes').innerHTML) + 1
}

function unlike_post(post) {
    var post_id = post.dataset.id
    fetch(`/like/${parseInt(post_id)}`, {
        method: 'PUT',
        body: JSON.stringify({
            like: false
        })
    })
    post.querySelector('.postLikes').innerHTML = parseInt(post.querySelector('.postLikes').innerHTML) - 1
}
