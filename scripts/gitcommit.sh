git add *
git status

echo Commit message\:
read commitMessage

git commit -m "$commitMessage"

git push origin master