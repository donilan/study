run:
	echo "Do nothing!"

clean:
	for file in `cat .gitignore`; do find . -name $$file -exec rm {} -rf \; ;done
