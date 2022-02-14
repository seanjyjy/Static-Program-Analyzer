<program>
	<procedure name=n1iwm>
		<stmts>
			<read>
				<var name=x></var>
			</read>
			<if>
				<eq>
					<var name=x></var>
					<const val=1></const>
				</eq>
				<stmts>
					<print>
						<var name=correct></var>
					</print>
					<while>
						<eq>
							<const val=1></const>
							<const val=1></const>
						</eq>
						<stmts>
							<print>
								<var name=correct3></var>
							</print>
						</stmts>
					</while>
					<print>
						<var name=correct2></var>
					</print>
				</stmts>
				<stmts>
					<print>
						<var name=wrong></var>
					</print>
					<while>
						<eq>
							<const val=2></const>
							<const val=2></const>
						</eq>
						<stmts>
							<print>
								<var name=wrong3></var>
							</print>
						</stmts>
					</while>
					<print>
						<var name=wrong2></var>
					</print>
				</stmts>
			</if>
			<print>
				<var name=y></var>
			</print>
		</stmts>
	</procedure>
</program>
