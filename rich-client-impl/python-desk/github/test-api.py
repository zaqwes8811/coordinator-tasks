from github import Github

g = Github("zaqwes8811", "a3fs4gd5h")

for repo in g.get_user().get_repos():
    print repo.name
    #repo.edit(has_wiki=False)

"""from pygithub3 import Github
from pygithub3.services.repos import *

gh = Github(login='zaqwes8811', password='a3fs4gd5h')"""
"""print gh

octocat = gh.users.get() # Auth required
print octocat

# 
octocat_followers = gh.users.followers.list().all()
print octocat_followers"""

#
"""
repo_service = Repo()
result = repo_service.list('zaqwes8811', type='owner')
for page in result:
    for resource in page:
        print resource

copitux = gh.users.get()
kennethreitz = gh.users.get('kennethreitz')

copitux_repos = gh.repos.list().all()
kennethreitz_repos = gh.repos.list('kennethreitz').all()"""