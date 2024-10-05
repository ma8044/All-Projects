from django.http import HttpResponse, HttpResponseRedirect
from django.shortcuts import render
from django.urls import reverse
import markdown2
from . import util
from django import forms
from random import randint

class NewWikiForm(forms.Form):
    q = forms.CharField(label="New Task")


def search(request):
    if request.method == "POST":

        form = NewWikiForm(request.POST)

        if form.is_valid():
            q=form.cleaned_data["q"]
            new_list = []
            for entry in util.list_entries():
                if entry.casefold() == q.casefold():
                    return HttpResponseRedirect(reverse('index')+"wiki/"+entry)
                if q.casefold() in entry.casefold():
                    new_list.append(entry)
            return render(request, "encyclopedia/index.html", {
                "entries": new_list,
                "title": "Search",
                "body": "Search Results for '" + q + "'" 
            })

def index(request):
    return render(request, "encyclopedia/index.html", {
        "title": "Encylopedia",
        "body": "All Pages",
        "entries": util.list_entries()
    })

def display(request, fileName):
    file = util.get_entry(fileName)
    if file:
        html = markdown2.markdown(file)
        return render(request, "encyclopedia/display.html", {
            "title": fileName,
            "body": html
        })
            
    else:
        return render(request, "encyclopedia/error.html",{
            "message": "Page not Found!"
        })

def new(request):
    if request.method == "POST":
        form = request.POST
        title = form["q"]
        content = form["content"]
        for entry in util.list_entries():
            if title.casefold() == entry.casefold():
                return render(request, "encyclopedia/error.html",{
                    "message": "Page Already Exists!"
                })
        util.save_entry(title, content)
        return HttpResponseRedirect(reverse('index')+"wiki/"+title)
    return render(request, "encyclopedia/newPage.html")

def edit(request, fileName):
    if request.method == "POST":
        content=request.POST["content"]
        util.save_entry(fileName, content)
        return HttpResponseRedirect(reverse('index')+"wiki/"+fileName)
    if request.method == "GET":
        content = util.get_entry(fileName)
        return render(request, "encyclopedia/edit.html", {
            "title": fileName,
            "content": content
        })
    
def random(request):
    entries = util.list_entries()
    file = entries[randint(0, len(entries)-1)]
    return HttpResponseRedirect(reverse('index')+"wiki/"+file)